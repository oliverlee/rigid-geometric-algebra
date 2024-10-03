"""
Rule for creating a fake venv from a requirements file.
Allows pyright to find modules with extraPaths.
"""

def _impl(rctx):
    rctx.file(
        "WORKSPACE.bazel",
        content = """\
workspace(name = {name})
        """.format(name = rctx.name),
        executable = False,
    )

    rctx.file(
        "BUILD.bazel",
        content = """\
load("{requirements}", "all_requirements")

filegroup(
    name = "all_files",
    srcs = glob(["**/*"]),
)
        """.format(
            requirements = rctx.attr.pip_requirements,
        ),
        executable = False,
    )

    rctx.file(
        "requirements.bash",
        content = r"""\
#!/usr/bin/env bash
set -euo pipefail
set -x

packages=$(
  grep "\"@{pyenv}//.*:pkg\",$" {requirements} \
  | sed  -e 's/.*\/\/\(.*\):pkg",/\1/')

echo "$packages"
        """.format(
            requirements = rctx.path(rctx.attr.pip_requirements),
            pyenv = rctx.attr.pip_requirements.workspace_name,
        ),
        executable = True,
    )

    result = rctx.execute(
        ["./requirements.bash"],
        timeout = 1,
    )
    if result.return_code != 0:
        fail(result.stderr)

    pyenv = rctx.attr.pip_requirements.workspace_name
    external = str(rctx.path(rctx.attr.pip_requirements)).rsplit("/", 2)[0]
    for pkg in result.stdout.splitlines():
        site_packages = "{external}/{pyenv}_{pkg}/site-packages".format(
            external = external,
            pyenv = pyenv,
            pkg = pkg,
        )

        result = rctx.execute(
            [
                "find",
                site_packages,
                "!",
                "-name",
                "*-*",
                "-maxdepth",
                "1",
                "-type",
                "d",
            ],
            timeout = 1,
        )
        modules = [
            p.rpartition("/")[-1]
            for p in result.stdout.strip().splitlines()
        ]

        for mod in modules:
            rctx.symlink(
                "{source}/{mod}".format(
                    source = site_packages,
                    mod = mod,
                ),
                "{pyenv}/site-packages/{mod}".format(
                    pyenv = pyenv,
                    mod = mod,
                ),
            )

local_python_fake_venv = repository_rule(
    implementation = _impl,
    local = True,
    attrs = {
        "pip_requirements": attr.label(
            doc = """
            Generated requirements file.
            """,
        ),
    },
)
