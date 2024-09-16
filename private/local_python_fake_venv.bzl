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
        timeout = 3,
    )
    if result.return_code != 0:
        fail(result.stderr)

    pyenv = rctx.attr.pip_requirements.workspace_name
    external = str(rctx.path(rctx.attr.pip_requirements)).rsplit("/", 2)[0]
    for pkg in result.stdout.splitlines():
        rctx.symlink(
            "{external}/{pyenv}_{pkg}/site-packages/{pkg}".format(
                external = external,
                pyenv = pyenv,
                pkg = pkg,
            ),
            "{pyenv}/site-packages/{pkg}".format(
                pyenv = pyenv,
                pkg = pkg,
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
