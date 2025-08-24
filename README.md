#Waffle

## What is Waffle?

Waffle is a playful systems language I’m building to learn compilers by doing.
It aims for **RAII by default**, **move-by-default semantics**, and **implicit borrows** (no `&`/`*`), with a
compile-time borrow checker and a tiny runtime. Codegen targets **LLVM**.

### Design goals

* **Safety without ceremony**: ownership + RAII, no raw pointers, implicit borrows, clear “one writer or many readers.”
* **Predictable semantics**: explicit evaluation order, statement-ternary, no hidden magic.
* **Move by default**: copying is intentional; returns are moves (RVO guaranteed).
* **Great errors**: friendly diagnostics with spans, notes, and fix-its.
* **Small runtime**: no GC; memory is released by scope (owners), not by a collector.
* **Interoperability**: `extern` and `extern "C"` for ABI-level linking.
* **Readable operators**: pure vs mutating operators are distinct; `Self` vs `self` makes intent obvious.

### Non-goals (for now)

* Garbage collection.
* Exceptions/stack unwinding (error handling will be library-level in later MVPs).
* Implicit heap allocation outside `Own<…>`.
* Undefined behavior as a “performance feature” — prefer well-defined, optimizable rules.

### Project stage

**Development Stage:** MVP-0 (core language skeleton) — parser + typing for primitives, ops, control flow, and Go-style
functions, *without* structs/interfaces/refs/generics\*.\*

**Release Stage:** –

---

### Visibility across packages (MVP-0)

* Top-level functions are **private to their package** unless marked `public`.
* Other packages may call only `public` functions.
* `extern` declares a function provided by another package or a foreign library; ABI can be specified with `extern "C"`.

```wfl
// in lib/io/print.wfl  (package lib.io)
public extern "C" func printf(int8) int32;
public func print_i32(int32 x) void { /* ... */ }

// in app/main.wfl  (package app)
use lib.io as io;
func main() int32 { io::print_i32(0); return 0; }
```

### Dependency graph & build

* The compiler constructs a **package DAG** using `use` declarations.
* **Cycles are illegal** (diagnostic points to the cycle).
* Build order is **topological**; each package’s files are compiled as a unit.
* Suggested CLI (sketch):

  * `waffle build <path-to-root-package>` — builds an executable from that folder as root.
  * `waffle check <path-to-root-package>` — type-check only.
  * `WAFFLE_PATH=/some/dir1:/some/dir2` — optional extra search paths for packages.

### Symbol mangling (MVP-0)

* External symbol name = `pkgid::funcname` by default (e.g., `lib.io::print_i32`).
* `extern "C"` keeps the exact symbol name (no mangling).

---

## MVP-1 Desired Specs (WIP)

### Built-in types

* **Integers**: `int8` `int16` `int32` `int64` `uint8` `uint16` `uint32` `uint64`
* **Bool**: `bool`
* **Floats**: `fp32` `fp64`

### Declaration and mutability

* **Immutable** by default
* Forms:

  * `T x;` — declare (uninitialized)
  * `mut T x;` — declare mutable
  * `T x = expr;` / `mut T x = expr;` — declare and initialize
  * `var x = expr;` / `mut var x = expr;` — declare and initialize with implicit type
* States:

  * `Declared` — write-only
  * `Assigned` — read and, if mutable, write
  * `Moved` — write-only if mutable
* Reading `Declared` / `Moved` is a compile-time error

### Lifetimes & destruction (RAII)

* A variable lives until the end of its declaring scope.
* **Drop order**: reverse declaration order within a scope.
* **Temporaries** live until the end of the full expression (usually the statement’s `;`).
* **Return by value = move** into the caller (RVO/NRVO guaranteed for aggregates/owners).
* At any scope exit, a value that owns memory may drop **only if it isn’t currently borrowed**.

### Heap ownership

* `Own<T>` and `Own<mut T>` allocate `T` on the heap and free it automatically when the owner goes out of scope (if not
  moved).
* **Borrowing capability**

  * `Own<T>` — can create immutable `Ref<T>`
  * `Own<mut T>` — can create immutable `Ref<T>` and mutable `Ref<mut T>`
* **Moves**: owners are **move-only**. Assigning/passing/returning an owner by value **moves** it; the source becomes
  `Moved`. Deep copies require explicit library code (not implicit).

### References (borrows)

* Types: `Ref<T>` (read-only), `Ref<mut T>` (read/write)
* **Cannot** make `Ref<Own<…>>` (or any ref to an owner handle). Always borrow the **underlying value** `T`.
* **Aliasing rule**: one writer **or** many readers:

  * Any number of `Ref<T>`, **or** exactly one `Ref<mut T>`.
  * A `Ref<mut T>` cannot coexist with any other ref.
* **Reborrow**: from `Ref<mut T>` you may create `Ref<T>`; while such **reborrows** exist, the mutable ref is **locked (
  read-only)**.
* **Auto-deref reads**: using a ref **where** `T` is expected performs a **copy** of `T`.

### Moves vs copies (surface behavior)

* **Move-by-default** for assignments, argument passing, and returns **from mutable bindings and move-only types**.
* From **immutable bindings**, reads behave like copies (they cannot be “consumed”).
* **Reading a moved value is illegal**; a moved binding becomes writable again **only** if it was declared `mut`.
* Common conversions:

  * `T → T`: **copy**
  * `T → Own<T>`: **alloc and copy** value into a new owner
  * `Own<T> → Own<T>`: **move** (only when not borrowed)
  * `Own<T> → T`: **copy** (creates an immutable borrow to read)
  * `Ref<T>` / `Ref<mut T> → T`: **copy** (auto-deref read)

### The `asRef` helper (stdlib)

A tiny generic that forces “read as immutable reference,” so you don’t accidentally move:

```txt
generic asRef<T>(x: Ref<T>) -> Ref<T> { return x; }
generic asRef<T>(x: Ref<mut T>) -> Ref<T> { return x; }  // reborrow; locks the mutable ref
```

* Passing a **plain** `T` or an **owner** to `asRef` implicitly creates an **immutable borrow** at the call site.
* Assigning the result to a value **copies** (auto-deref). Assigning to an owner **allocates and copies**.

Examples:

```txt
mut int x = 3;
int a = asRef(x);          // borrow + copy; x not moved

Own<int> o1 = 7;
Own<int> o2 = asRef(o1);   // alloc + copy from *o1

Own<mut int> om = 10;
Ref<mut int> m = om;
int v = asRef(m);          // reborrow imm (locks m), then copy
```

### Functions & calls (essentials)

* Passing **plain values** (`int`, structs) by value — **copy**.
* Passing **owners** (`Own<…>`) by value — **move** (caller’s binding becomes `Moved`).
* Passing **refs** by value — **move the handle**; the underlying borrow must remain valid for the call’s duration.
* If a parameter expects `Ref<T>` and you have `Ref<mut T>`, the compiler inserts a **temporary reborrow** (locking the
  mutable ref during the call).
* Returning by value always moves into the caller:

  * Returning an **owner** requires it be **not borrowed** at `return`.
  * Returning a **ref** is allowed only if it is tied to an input that outlives the caller (never to a callee
    local/temporary).

### Temporaries & expression lifetimes

* A temporary created within a **call/initializer** lives **until the end of that full expression**
  `checkGreen(asRef(color));  // temporary created inside asRef dies after ')')`
* You cannot return or store a ref to such a temporary object beyond the statement (compile-time error).

### Typical errors (diagnostics)

* Use of **uninitialized** (`Declared`) or **moved** value.
* **Owner drops while borrowed** — a reference to its value is still alive at scope exit.
* **Borrow rule violation** — creating a mutable ref while immutable refs exist, or vice versa.
* **Mutating via a locked mutable ref** — while immutable reborrows exist.
* **Escaping reference** — returning/keeping a ref that would outlive its source.

### Quick examples

```txt
// copies between stack values
int i = 3;
int a = i;                 // copy

// build an owner from a value (alloc + copy)
Own<int> o1 = 3;           // from literal
Own<int> o2 = asRef(i);   // from stack value

// move ownership
Own<int> p = 3;
Own<int> q = p;            // move; p is now moved

// read from an owner (copy)
Own<int> r = 9;
int v = r;                 // copy *r (no exclusive borrow active)

// borrow rules
Own<mut int> s = 10;
Ref<mut int> m = s;        // exclusive mutable borrow
int t = m;                 // copy via m
Ref<int> rr = m;           // reborrow imm; m is locked until rr ends
// m = 42;                 // ERROR: m is locked here (write forbidden while reborrows live)
```