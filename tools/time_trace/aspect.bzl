"""
Aspect to generate a Clang time trace files for a C++ compilation
"""

load("@rules_cc//cc:action_names.bzl", "ACTION_NAMES")
load("@rules_cc//cc:find_cc_toolchain.bzl", "CC_TOOLCHAIN_TYPE", "find_cc_toolchain")

visibility("//...")

def _format_label(label):
    return "{}//{}:{}".format(
        "@" + label.workspace_name if label.workspace_name else "",
        label.package,
        label.name,
    )

def _get_compile_command(
        rule_ctx,
        compilation_ctx,
        toolchain,
        extra_flags = None,
        action_name = ACTION_NAMES.cpp_compile):
    if action_name != ACTION_NAMES.cpp_compile:
        fail("'{}' not currently supported".format(action_name))

    fragment_flags = rule_ctx.fragments.cpp.cxxopts + rule_ctx.fragments.cpp.copts

    feature_configuration = cc_common.configure_features(
        ctx = rule_ctx,
        cc_toolchain = toolchain,
        requested_features = rule_ctx.features,
        unsupported_features = rule_ctx.disabled_features,
    )

    compile_variables = cc_common.create_compile_variables(
        cc_toolchain = toolchain,
        feature_configuration = feature_configuration,
        user_compile_flags = fragment_flags + (extra_flags or []),
        include_directories = compilation_ctx.includes,
        quote_include_directories = compilation_ctx.quote_includes,
        system_include_directories = depset(
            transitive = [
                compilation_ctx.system_includes,
                compilation_ctx.external_includes,
            ],
        ),
        framework_include_directories = compilation_ctx.framework_includes,
        preprocessor_defines = depset(
            transitive = [
                compilation_ctx.defines,
                compilation_ctx.local_defines,
            ],
        ),
    )

    compiler = cc_common.get_tool_for_action(
        feature_configuration = feature_configuration,
        action_name = action_name,
    )

    options = cc_common.get_memory_inefficient_command_line(
        feature_configuration = feature_configuration,
        action_name = action_name,
        variables = compile_variables,
    )

    return struct(
        compiler = compiler,
        options = options,
    )

def _impl(target, ctx):
    toolchain = find_cc_toolchain(ctx)

    compilation_context = target[CcInfo].compilation_context

    compile_command = _get_compile_command(
        ctx,
        compilation_context,
        toolchain,
        getattr(ctx.rule.attr, "copts", []),
    )

    outputs = []
    for src in ctx.rule.files.srcs:
        [object, mangled_json] = [
            ctx.actions.declare_file(
                "_time_trace/{}/{}.{}".format(
                    ctx.label.name,
                    src.basename,
                    ext,
                ),
            )
            for ext in ["o", "json"]
        ]

        arguments = list(compile_command.options)
        arguments.extend([
            "-ftime-trace",
            "-c",
            src.path,
            "-o",
            object.path,
        ])

        ctx.actions.run(
            inputs = depset(
                direct = [src],
                transitive = [compilation_context.headers],
            ),
            outputs = [mangled_json, object],
            tools = toolchain.all_files,
            executable = compile_command.compiler,
            arguments = arguments,
            use_default_shell_env = True,
            mnemonic = "TimeTrace",
            progress_message = "Creating time trace for {}".format(
                _format_label(target.label),
            ),
            execution_requirements = ctx.attr._execution_requirements,
        )

        outputs.extend([mangled_json])

    return [OutputGroupInfo(time_trace = outputs)]

def make_time_trace_aspect(
        *,
        execution_requirements = None):
    return aspect(
        implementation = _impl,
        attrs = {
            "_execution_requirements": attr.string_dict(
                default = execution_requirements or {},
            ),
        },
        fragments = ["cpp"],
        required_providers = [CcInfo],
        toolchains = [CC_TOOLCHAIN_TYPE],
    )

time_trace_aspect = make_time_trace_aspect(
    execution_requirements = {
        "no-remote": "1",
    },
)
