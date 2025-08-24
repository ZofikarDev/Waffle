#

```txt
mut int x = 3;
int a = asRef(x); // borrow + copy; x not moved


Own<int> o1 = 7;
Own<int> o2 = asRef(o1); // alloc + copy from *o1


Own<mut int> om = 10;
Ref<mut int> m = om;
int v = asRef(m); // reborrow imm (locks m), then copy
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
  `checkGreen(asRef(color)); // temporary created inside asRef dies after ')')`
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
int a = i; // copy


// build an owner from a value (alloc + copy)
Own<int> o1 = 3; // from literal
Own<int> o2 = asRef(i); // from stack value


// move ownership
Own<int> p = 3;
Own<int> q = p; // move; p is now moved


// read from an owner (copy)
Own<int> r = 9;
int v = r; // copy *r (no exclusive borrow active)


// borrow rules
Own<mut int> s = 10;
Ref<mut int> m = s; // exclusive mutable borrow
int t = m; // copy via m
Ref<int> rr = m; // reborrow imm; m is locked until rr ends
// m = 42; // ERROR: m is locked here (write forbidden while reborrows live)
```