
function(load_sources_list result path extensions)
	set(sources "")
	foreach(ext IN LISTS extensions)
		file(GLOB_RECURSE additional CONFIGURE_DEPENDS ${path}/*${ext})
		list(APPEND sources ${additional})
	endforeach()

	list(REMOVE_DUPLICATES sources)
	source_group(${path} FILES ${sources})

	set(${result} ${sources} PARENT_SCOPE)
endfunction()

# Call this function at the end of a directory scope to assign a folder to
# targets created in that directory. Utility targets will be assigned to the
# UtilityTargets folder, otherwise to the ${name}Targets folder. If a target
# already has a folder assigned, then that target will be skipped.
function(add_folders name)
	if (NOT __USE_FOLDERS_INITIALIZED)
		set_property(GLOBAL PROPERTY USE_FOLDERS YES)
		set(__USE_FOLDERS_INITIALIZED ON)
	endif()

	get_property(targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
	foreach(target IN LISTS targets)
		get_property(folder TARGET "${target}" PROPERTY FOLDER)
		if(DEFINED folder)
			continue()
		endif()
		set(folder Utility)
		get_property(type TARGET "${target}" PROPERTY TYPE)
		if(NOT type STREQUAL "UTILITY")
			set(folder "${name}")
		endif()
		set_property(TARGET "${target}" PROPERTY FOLDER "${folder}Targets")
	endforeach()
endfunction()
