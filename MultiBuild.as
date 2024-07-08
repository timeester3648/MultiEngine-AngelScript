void main(MultiBuild::Workspace& workspace) {	
	auto project = workspace.create_project(".");
	auto properties = project.properties();

	project.name("AngelScript");
	properties.binary_object_kind(MultiBuild::BinaryObjectKind::eStaticLib);
	properties.defines("ANGELSCRIPT_EXPORT");

	project.include_own_required_includes(true);
	project.add_required_project_include({
		"./sdk/add_on",
		"./sdk/angelscript/include"
	});

	properties.files({
		"./sdk/angelscript/include/**.h",
		"./sdk/angelscript/source/**.h",
		"./sdk/angelscript/source/**.cpp",

		"./sdk/add_on/**.h",
		"./sdk/add_on/**.cpp"
	});

	properties.excluded_files({
		"./sdk/add_on/autowrapper/**.h",
		"./sdk/add_on/autowrapper/**.cpp"
	});

	{
		MultiBuild::ScopedFilter _(project, "project.compiler:VisualCpp");
		properties.disable_warnings({ "4468", "4244", "4996" });

		properties.files("./sdk/angelscript/source/as_callfunc_x64_msvc_asm.asm");
	}

	{
		MultiBuild::ScopedFilter _(project, "project.compiler:VisualCpp && file.path:**as_callfunc_x64_msvc_asm.asm");
		properties.build_message("Compiling {:file.name}");

		properties.build_commands({
			"ml64 /c /Cp /Cx /nologo /Zi /Fo{:project.obj_dir}/{:file.stem}.obj {:file.path}"
		});

		properties.build_outputs({
			"{:project.obj_dir}/{:file.stem}.obj"
		});
	}
}