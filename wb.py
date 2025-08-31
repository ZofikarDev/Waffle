#!/usr/bin/env python3
import argparse
import os
import subprocess
import sys
from pathlib import Path

DOCKER_IMAGE_NAME = "ghcr.io/zofikardev/waffle-dev"
DOCKER_VERSION_FILE = Path(__file__).parent / "DOCKER_VERSION"

try:
    with open(DOCKER_VERSION_FILE) as f:
        DOCKER_VERSION = f.read().strip()
except FileNotFoundError:
    print(f"[error] {DOCKER_VERSION_FILE} not found")
    sys.exit(1)

DOCKER_IMAGE = f"{DOCKER_IMAGE_NAME}:{DOCKER_VERSION}"
DOCKERFILE = "Dockerfile"

DEFAULT_BUILD_DIR = "build"


def run(cmd, **kwargs):
    print("+", " ".join(cmd))
    return subprocess.run(cmd, **kwargs)


def docker_image_exists():
    result = subprocess.run(
        ["docker", "images", "-q", DOCKER_IMAGE],
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
    )
    return bool(result.stdout.strip())


def docker_pull():
    return run(["docker", "pull", DOCKER_IMAGE])


def ensure_image():
    result = subprocess.run(
        ["docker", "image", "inspect", DOCKER_IMAGE],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL
    )
    if result.returncode != 0:
        # Try pull first
        if run(["docker", "pull", DOCKER_IMAGE]).returncode != 0:
            run([
                "docker", "build", "-t", DOCKER_IMAGE,
                "-f", DOCKERFILE, "."
            ], check=True)


def docker_run(extra_args, check=True):
    try:
        return run([
                       "docker", "run", "--rm",
                       "-v", f"{os.path.abspath(os.getcwd()).replace('\\', '/')}" + ":/workspace",
                       "-w", "/workspace",
                       f"{DOCKER_IMAGE}"
                   ] + extra_args, check=check)
    except subprocess.CalledProcessError as e:
        print(f"[error] {e}")
        return e


# ---- Project commands ----

def cmd_build(args):
    ensure_image()

    if docker_run(["cmake", "--build", args.build_dir, "-j"]).returncode != 0:
        sys.exit(1)


def cmd_configure(args):
    ensure_image()
    cmd = [
        "cmake", "-S", ".", "-B", args.build_dir,
        "-G", "Ninja"
    ]
    if docker_run(cmd).returncode != 0:
        sys.exit(1)


def cmd_test(args):
    ensure_image()
    test_cmd = ["ctest", "--test-dir", args.build_dir]
    if args.filter:
        test_cmd += ["-R", args.filter]
    if docker_run(test_cmd).returncode != 0:
        sys.exit(1)


def cmd_format(args):
    ensure_image()
    if args.mode == "check":
        cmd = "clang-format --dry-run --Werror $(find Libs src -type f \\( -name '*.cpp' -o -name '*.h' \\))"
    else:
        cmd = "clang-format -i $(find Libs src -type f \\( -name '*.cpp' -o -name '*.h' \\))"
    result = docker_run(["bash", "-c", cmd], check=False)
    sys.exit(result.returncode)


# ---- Image commands ----

def image_exists_remote():
    result = subprocess.run(
        ["docker", "manifest", "inspect", DOCKER_IMAGE],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return result.returncode == 0


def cmd_image_push(args):
    if image_exists_remote():
        if not args.update:
            print(f"[error] {DOCKER_IMAGE} already exists in registry. Use --update to push anyway.")
            sys.exit(1)
    elif args.update:
        print(f"[error] {DOCKER_IMAGE} does not exist in registry. Can not update it.")
        sys.exit(1)

    run(["docker", "build", "-t", DOCKER_IMAGE, "-f", DOCKERFILE, "."])
    run(["docker", "push", DOCKER_IMAGE])


def cmd_image_pull(args):
    sys.exit(docker_pull().returncode)


def cmd_image_build(args):
    run(["docker", "build", "-t", DOCKER_IMAGE, "-f", DOCKERFILE, "."])


def cmd_image_shell(args):
    try:
        subprocess.run([
            "docker", "run", "--rm", "-it",
            "-v", f"{os.path.abspath(os.getcwd()).replace('\\', '/')}" + ":/workspace",
            "-w", "/workspace",
            f"{DOCKER_IMAGE}",
            "bash"
        ], check=True)
    except subprocess.CalledProcessError as e:
        print(f"[error] exited with {e.returncode}")



# ---- CLI setup ----

def main():
    parser = argparse.ArgumentParser(description="Waffle Docker utility")
    sub = parser.add_subparsers(dest="command", required=True)

    # Project commands
    p_configure = sub.add_parser("configure", help="Configure project")
    p_configure.add_argument("--build-dir", default=DEFAULT_BUILD_DIR, help="Build directory")
    p_configure.set_defaults(func=cmd_configure)

    p_build = sub.add_parser("build", help="Build project")
    p_build.add_argument("--build-dir", default=DEFAULT_BUILD_DIR, help="Build directory")
    p_build.set_defaults(func=cmd_build)

    p_test = sub.add_parser("test", help="Run tests")
    p_test.add_argument("--filter", help="Run only tests matching regex")
    p_test.add_argument("--build-dir", default=DEFAULT_BUILD_DIR, help="Build directory")
    p_test.set_defaults(func=cmd_test)

    p_fmt = sub.add_parser("format", help="Format code")
    fmt_sub = p_fmt.add_subparsers(dest="mode", required=True)
    fmt_sub.add_parser("check", help="Check formatting").set_defaults(func=cmd_format)
    fmt_sub.add_parser("fix", help="Fix formatting").set_defaults(func=cmd_format)

    # Image commands group
    p_image = sub.add_parser("image", help="Docker image management")
    image_sub = p_image.add_subparsers(dest="subcommand", required=True)

    image_push = image_sub.add_parser("push", help="Build and push docker image")
    image_push.add_argument("--update", action="store_true", help="Update existing image")
    image_push.set_defaults(func=cmd_image_push)
    image_sub.add_parser("pull", help="Pull docker image").set_defaults(func=cmd_image_pull)
    image_sub.add_parser("build", help="Build docker image").set_defaults(func=cmd_image_build)
    image_sub.add_parser("shell", help="Run shell in docker image").set_defaults(func=cmd_image_shell)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    sys.exit(main())
