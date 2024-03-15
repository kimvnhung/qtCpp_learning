macro(configure_files srcDir destDir filter)
    message(STATUS "Configuring directory ${destDir}")
    make_directory(${destDir})

    file(GLOB templateFiles RELATIVE ${srcDir} "${srcDir}/*")

    foreach(templateFile ${templateFiles})
        string(FIND ${templateFile} ${filter} RESULT)

        if(RESULT EQUAL -1)
            continue()
        endif()

        set(srcTemplatePath ${srcDir}/${templateFile})
        message("srcTempath " ${srcTemplatePath})

        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                ${srcTemplatePath}
                ${destDir}/${templateFile}
                @ONLY)
        else()
            configure_files(${srcTemplatePath} ${destDir}/${templateFile} ${filter})
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)
