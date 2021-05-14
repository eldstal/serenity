cmake_minimum_required (VERSION 3.0)
project (Lagom)

if (NOT ENABLE_OSS_FUZZ)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -Wno-literal-suffix -O2 -Wall -Wextra -std=c++2a -fPIC -g -Wno-deprecated-copy")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -fPIC -g -Wno-deprecated-copy")
endif()

if (ENABLE_ADDRESS_SANITIZER)
    add_definitions(-fsanitize=address -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=address")
endif()

if (ENABLE_MEMORY_SANITIZER)
    add_definitions(-fsanitize=memory -fsanitize-memory-track-origins -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=memory -fsanitize-memory-track-origins")
endif()

if (ENABLE_UNDEFINED_SANITIZER)
    add_definitions(-fsanitize=undefined -fno-sanitize=vptr -fno-omit-frame-pointer)
    set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=undefined -fno-sanitize=vptr")
endif()

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang$")
    # Clang's default constexpr-steps limit is 1048576(2^20), GCC doesn't have one
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-overloaded-virtual -Wno-user-defined-literals -fconstexpr-steps=16777216")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASSERT_IS_EXIT")

    if (ENABLE_FUZZER_SANITIZER)
        add_definitions(-fsanitize=fuzzer -fno-omit-frame-pointer)
        set(LINKER_FLAGS "${LINKER_FLAGS} -fsanitize=fuzzer")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-expansion-to-defined -Wno-literal-suffix")
endif()

# These are here to support Fuzzili builds further down the directory stack
set(ORIGINAL_CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(ORIGINAL_CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(ORIGINAL_CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

file(GLOB AK_SOURCES CONFIGURE_DEPENDS "../../AK/*.cpp")
file(GLOB AK_TEST_SOURCES CONFIGURE_DEPENDS "../../Tests/AK/*.cpp")
file(GLOB LIBARCHIVE_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibArchive/*.cpp")
file(GLOB LIBAUDIO_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibAudio/*.cpp")
list(REMOVE_ITEM LIBAUDIO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../Userland/Libraries/LibAudio/ClientConnection.cpp")
file(GLOB LIBREGEX_LIBC_SOURCES "../../Userland/Libraries/LibRegex/C/Regex.cpp")
file(GLOB LIBREGEX_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibRegex/*.cpp")
file(GLOB LIBREGEX_TESTS CONFIGURE_DEPENDS "../../Tests/LibRegex/*.cpp")
file(GLOB LIBCORE_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibCore/*.cpp")
file(GLOB LIBELF_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibELF/*.cpp")
# There's no way we can reliably make this cross platform
list(REMOVE_ITEM LIBELF_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../Userland/Libraries/LibELF/DynamicLinker.cpp")
file(GLOB LIBGEMINI_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibGemini/*.cpp")
file(GLOB LIBGFX_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibGfx/*.cpp")
file(GLOB LIBGUI_GML_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibGUI/GML*.cpp")
list(REMOVE_ITEM LIBGUI_GML_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../Userland/Libraries/LibGUI/GMLSyntaxHighlighter.cpp")
file(GLOB LIBHTTP_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibHTTP/*.cpp")
file(GLOB LIBIPC_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibIPC/*.cpp")
file(GLOB LIBLINE_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibLine/*.cpp")
file(GLOB LIBMARKDOWN_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibMarkdown/*.cpp")
file(GLOB LIBX86_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibX86/*.cpp")
file(GLOB LIBJS_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibJS/*.cpp")
file(GLOB LIBJS_SUBDIR_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibJS/*/*.cpp")
file(GLOB LIBCOMPRESS_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibCompress/*.cpp")
file(GLOB LIBCOMPRESS_TESTS CONFIGURE_DEPENDS "../../Tests/LibCompress/*.cpp")
file(GLOB LIBCRYPTO_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibCrypto/*.cpp")
file(GLOB LIBCRYPTO_SUBDIR_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibCrypto/*/*.cpp")
file(GLOB LIBCRYPTO_SUBSUBDIR_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibCrypto/*/*/*.cpp")
file(GLOB LIBTLS_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibTLS/*.cpp")
file(GLOB LIBTTF_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibTTF/*.cpp")
file(GLOB LIBTEXTCODEC_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibTextCodec/*.cpp")
file(GLOB SHELL_SOURCES CONFIGURE_DEPENDS "../../Userland/Shell/*.cpp")
file(GLOB SHELL_TESTS CONFIGURE_DEPENDS "../../Userland/Shell/Tests/*.sh")
list(FILTER SHELL_SOURCES EXCLUDE REGEX ".*main.cpp$")
file(GLOB LIBSQL_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibSQL/*.cpp")
list(REMOVE_ITEM LIBSQL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/../../Userland/Libraries/LibSQL/SyntaxHighlighter.cpp")
file(GLOB LIBSQL_TEST_SOURCES CONFIGURE_DEPENDS "../../Tests/LibSQL/*.cpp")
file(GLOB LIBWEB_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibWeb/*.cpp" "../../Userland/Libraries/LibWeb/*/*.cpp" "../../Userland/Libraries/LibWeb/*/*/*.cpp")

file(GLOB LIBTEST_SOURCES CONFIGURE_DEPENDS "../../Userland/Libraries/LibTest/*.cpp")
list(FILTER LIBTEST_SOURCES EXCLUDE REGEX ".*Main.cpp$")
file(GLOB LIBTEST_MAIN CONFIGURE_DEPENDS "../../Userland/Libraries/LibTest/TestMain.cpp")

set(LAGOM_REGEX_SOURCES ${LIBREGEX_LIBC_SOURCES} ${LIBREGEX_SOURCES})
set(LAGOM_CORE_SOURCES ${AK_SOURCES} ${LIBCORE_SOURCES})
set(LAGOM_MORE_SOURCES ${LIBARCHIVE_SOURCES} ${LIBAUDIO_SOURCES} ${LIBELF_SOURCES} ${LIBIPC_SOURCES} ${LIBLINE_SOURCES} ${LIBJS_SOURCES} ${LIBJS_SUBDIR_SOURCES} ${LIBX86_SOURCES} ${LIBCRYPTO_SOURCES} ${LIBCOMPRESS_SOURCES} ${LIBCRYPTO_SUBDIR_SOURCES} ${LIBCRYPTO_SUBSUBDIR_SOURCES} ${LIBTLS_SOURCES} ${LIBTTF_SOURCES} ${LIBTEXTCODEC_SOURCES} ${LIBMARKDOWN_SOURCES} ${LIBGEMINI_SOURCES} ${LIBGFX_SOURCES} ${LIBGUI_GML_SOURCES} ${LIBHTTP_SOURCES} ${LAGOM_REGEX_SOURCES} ${SHELL_SOURCES} ${LIBSQL_SOURCES} ${LIBWEB_SOURCES})
set(LAGOM_TEST_SOURCES ${LIBTEST_SOURCES})

# FIXME: This is a hack, because the lagom stuff can be build individually or
#        in combination with the system, we generate two Debug.h files. One in
#        Build/AK/Debug.h and the other in Build/Meta/Lagom/AK/Debug.h.
configure_file(../../AK/Debug.h.in AK/Debug.h @ONLY)
configure_file(../../Kernel/Debug.h.in Kernel/Debug.h @ONLY)

include_directories(../../)
include_directories(../../Userland/)
include_directories(../../Userland/Libraries/)
include_directories(${CMAKE_BINARY_DIR})

# Hack for the autogenerated headers in LibWeb
include_directories(${CMAKE_BINARY_DIR}/Userland/Libraries/)

# Hacks for various other automatically generated files
include_directories(${CMAKE_BINARY_DIR}/Userland/Services/)

add_library(LagomCore ${LAGOM_CORE_SOURCES})


function(libweb_js_wrapper_dep class)
  get_filename_component(basename ${class} NAME)

  # Header files can be auto-generated no problem
  add_dependencies(Lagom generate_${basename}Wrapper.h)
  add_dependencies(Lagom generate_${basename}Constructor.h)
  add_dependencies(Lagom generate_${basename}Prototype.h)

  #add_dependencies(Lagom generate_${basename}Wrapper.cpp)
  #add_dependencies(Lagom generate_${basename}Constructor.cpp)
  #add_dependencies(Lagom generate_${basename}Prototype.cpp)

  # The cpp files need this hackery to be included in the sources of Lagom and actually be compiled
  #add_custom_command(
  #    OUTPUT Generated/hax_${basename}Wrapper.cpp
  #    COMMAND echo "-----------------------------------------------------------------"
  #    COMMAND cp ${CMAKE_BINARY_DIR}/Userland/Libraries/LibWeb/Bindings/${basename}Wrapper.cpp Generated/hax_${basename}Wrapper.cpp
  #    COMMAND pwd
  #    COMMAND echo cp ${CMAKE_BINARY_DIR}/Userland/Libraries/LibWeb/Bindings/${basename}Wrapper.cpp Generated/hax_${basename}Wrapper.cpp
  #    DEPENDS generate_${basename}Wrapper.cpp
  #)
  #add_custom_target(target_hax_${basename}Wrapper ALL DEPENDS ${CMAKE_BINARY_DIR}/Meta/Lagom/Generated/hax_${basename}Wrapper.cpp)
  #add_dependencies(Lagom target_hax_${basename}Wrapper)

  #add_custom_command(
  #    OUTPUT Generated/hax_${basename}Constructor.cpp
  #    COMMAND echo "-----------------------------------------------------------------"
  #    COMMAND cp ${CMAKE_BINARY_DIR}/Userland/Libraries/LibWeb/Bindings/${basename}Constructor.cpp Generated/hax_${basename}Wrapper.cpp
  #    COMMAND pwd
  #    COMMAND echo cp ${CMAKE_BINARY_DIR}/Userland/Libraries/LibWeb/Bindings/${basename}Constructor.cpp Generated/hax_${basename}Wrapper.cpp
  #    DEPENDS generate_${basename}Constructor.cpp
  #)
  #add_custom_target(target_hax_${basename}Constructor ALL DEPENDS ${CMAKE_BINARY_DIR}/Meta/Lagom/Generated/hax_${basename}Wrapper.cpp)
  #add_dependencies(Lagom target_hax_${basename}Constructor)

  #add_custom_command(
  #    OUTPUT Generated/hax_${basename}Prototype.cpp
  #    COMMAND echo "-----------------------------------------------------------------"
  #    COMMAND cp ${CMAKE_BINARY_DIR}/Userland/Libraries/LibWeb/Bindings/${basename}Prototype.cpp Generated/hax_${basename}Wrapper.cpp
  #    COMMAND pwd
  #    COMMAND echo cp ${CMAKE_BINARY_DIR}/Userland/Libraries/LibWeb/Bindings/${basename}Prototype.cpp Generated/hax_${basename}Wrapper.cpp
  #    DEPENDS generate_${basename}Prototype.cpp
  #)
  #add_custom_target(target_hax_${basename}Prototype ALL DEPENDS ${CMAKE_BINARY_DIR}/Meta/Lagom/Generated/hax_${basename}Wrapper.cpp)
  #add_dependencies(Lagom target_hax_${basename}Prototype)

endfunction()


if (BUILD_LAGOM)
    add_library(Lagom $<TARGET_OBJECTS:LagomCore> ${LAGOM_MORE_SOURCES})

    # The autogenerated stuff for LibProtocol etc
    add_dependencies(Lagom generate_ImageDecoderServerEndpoint.h)
    add_dependencies(Lagom generate_ImageDecoderClientEndpoint.h)
    add_dependencies(Lagom generate_SymbolServerEndpoint.h)
    add_dependencies(Lagom generate_SymbolClientEndpoint.h)
    add_dependencies(Lagom generate_ClipboardServerEndpoint.h)
    add_dependencies(Lagom generate_ClipboardClientEndpoint.h)
    add_dependencies(Lagom generate_LookupServerEndpoint.h)
    add_dependencies(Lagom generate_LookupClientEndpoint.h)
    add_dependencies(Lagom generate_AudioServerEndpoint.h)
    add_dependencies(Lagom generate_AudioClientEndpoint.h)
    add_dependencies(Lagom generate_WebContentServerEndpoint.h)
    add_dependencies(Lagom generate_WebContentClientEndpoint.h)
    add_dependencies(Lagom generate_WebSocketServerEndpoint.h)
    add_dependencies(Lagom generate_WebSocketClientEndpoint.h)
    add_dependencies(Lagom generate_RequestServerEndpoint.h)
    add_dependencies(Lagom generate_RequestClientEndpoint.h)
    add_dependencies(Lagom generate_NotificationServerEndpoint.h)
    add_dependencies(Lagom generate_NotificationClientEndpoint.h)
    add_dependencies(Lagom generate_LaunchServerEndpoint.h)
    add_dependencies(Lagom generate_LaunchClientEndpoint.h)
    add_dependencies(Lagom generate_WindowServerEndpoint.h)
    add_dependencies(Lagom generate_WindowClientEndpoint.h)
    add_dependencies(Lagom generate_WindowManagerServerEndpoint.h)
    add_dependencies(Lagom generate_WindowManagerClientEndpoint.h)


    # The autogenerated files in LibWeb
    add_dependencies(Lagom generate_PropertyID.h)
    add_dependencies(Lagom generate_ValueID.h)

    libweb_js_wrapper_dep(CSS/CSSStyleDeclaration)
    libweb_js_wrapper_dep(CSS/CSSStyleSheet)
    libweb_js_wrapper_dep(CSS/Screen)
    libweb_js_wrapper_dep(CSS/StyleSheet)
    libweb_js_wrapper_dep(CSS/StyleSheetList)
    libweb_js_wrapper_dep(DOM/CharacterData)
    libweb_js_wrapper_dep(DOM/Comment)
    libweb_js_wrapper_dep(DOM/Document)
    libweb_js_wrapper_dep(DOM/DocumentFragment)
    libweb_js_wrapper_dep(DOM/DocumentType)
    libweb_js_wrapper_dep(DOM/DOMException)
    libweb_js_wrapper_dep(DOM/DOMImplementation)
    libweb_js_wrapper_dep(DOM/Element)
    libweb_js_wrapper_dep(DOM/Event)
    libweb_js_wrapper_dep(DOM/EventTarget)
    libweb_js_wrapper_dep(DOM/HTMLCollection)
    libweb_js_wrapper_dep(DOM/ProcessingInstruction)
    libweb_js_wrapper_dep(DOM/ShadowRoot)
    libweb_js_wrapper_dep(DOM/Node)
    libweb_js_wrapper_dep(DOM/Range)
    libweb_js_wrapper_dep(DOM/Text)
    libweb_js_wrapper_dep(HTML/CanvasRenderingContext2D)
    libweb_js_wrapper_dep(HTML/CloseEvent)
    libweb_js_wrapper_dep(HTML/HTMLAnchorElement)
    libweb_js_wrapper_dep(HTML/HTMLAreaElement)
    libweb_js_wrapper_dep(HTML/HTMLAudioElement)
    libweb_js_wrapper_dep(HTML/HTMLBaseElement)
    libweb_js_wrapper_dep(HTML/HTMLBodyElement)
    libweb_js_wrapper_dep(HTML/HTMLBRElement)
    libweb_js_wrapper_dep(HTML/HTMLButtonElement)
    libweb_js_wrapper_dep(HTML/HTMLCanvasElement)
    libweb_js_wrapper_dep(HTML/HTMLDataElement)
    libweb_js_wrapper_dep(HTML/HTMLDataListElement)
    libweb_js_wrapper_dep(HTML/HTMLDetailsElement)
    libweb_js_wrapper_dep(HTML/HTMLDialogElement)
    libweb_js_wrapper_dep(HTML/HTMLDirectoryElement)
    libweb_js_wrapper_dep(HTML/HTMLDivElement)
    libweb_js_wrapper_dep(HTML/HTMLDListElement)
    libweb_js_wrapper_dep(HTML/HTMLElement)
    libweb_js_wrapper_dep(HTML/HTMLEmbedElement)
    libweb_js_wrapper_dep(HTML/HTMLFieldSetElement)
    libweb_js_wrapper_dep(HTML/HTMLFontElement)
    libweb_js_wrapper_dep(HTML/HTMLFormElement)
    libweb_js_wrapper_dep(HTML/HTMLFrameElement)
    libweb_js_wrapper_dep(HTML/HTMLFrameSetElement)
    libweb_js_wrapper_dep(HTML/HTMLHeadElement)
    libweb_js_wrapper_dep(HTML/HTMLHeadingElement)
    libweb_js_wrapper_dep(HTML/HTMLHRElement)
    libweb_js_wrapper_dep(HTML/HTMLHtmlElement)
    libweb_js_wrapper_dep(HTML/HTMLIFrameElement)
    libweb_js_wrapper_dep(HTML/HTMLImageElement)
    libweb_js_wrapper_dep(HTML/HTMLInputElement)
    libweb_js_wrapper_dep(HTML/HTMLLabelElement)
    libweb_js_wrapper_dep(HTML/HTMLLegendElement)
    libweb_js_wrapper_dep(HTML/HTMLLIElement)
    libweb_js_wrapper_dep(HTML/HTMLLinkElement)
    libweb_js_wrapper_dep(HTML/HTMLMapElement)
    libweb_js_wrapper_dep(HTML/HTMLMarqueeElement)
    libweb_js_wrapper_dep(HTML/HTMLMediaElement)
    libweb_js_wrapper_dep(HTML/HTMLMenuElement)
    libweb_js_wrapper_dep(HTML/HTMLMetaElement)
    libweb_js_wrapper_dep(HTML/HTMLMeterElement)
    libweb_js_wrapper_dep(HTML/HTMLModElement)
    libweb_js_wrapper_dep(HTML/HTMLObjectElement)
    libweb_js_wrapper_dep(HTML/HTMLOListElement)
    libweb_js_wrapper_dep(HTML/HTMLOptGroupElement)
    libweb_js_wrapper_dep(HTML/HTMLOptionElement)
    libweb_js_wrapper_dep(HTML/HTMLOutputElement)
    libweb_js_wrapper_dep(HTML/HTMLParagraphElement)
    libweb_js_wrapper_dep(HTML/HTMLParamElement)
    libweb_js_wrapper_dep(HTML/HTMLPictureElement)
    libweb_js_wrapper_dep(HTML/HTMLPreElement)
    libweb_js_wrapper_dep(HTML/HTMLProgressElement)
    libweb_js_wrapper_dep(HTML/HTMLQuoteElement)
    libweb_js_wrapper_dep(HTML/HTMLScriptElement)
    libweb_js_wrapper_dep(HTML/HTMLSelectElement)
    libweb_js_wrapper_dep(HTML/HTMLSlotElement)
    libweb_js_wrapper_dep(HTML/HTMLSourceElement)
    libweb_js_wrapper_dep(HTML/HTMLSpanElement)
    libweb_js_wrapper_dep(HTML/HTMLStyleElement)
    libweb_js_wrapper_dep(HTML/HTMLTableCaptionElement)
    libweb_js_wrapper_dep(HTML/HTMLTableCellElement)
    libweb_js_wrapper_dep(HTML/HTMLTableColElement)
    libweb_js_wrapper_dep(HTML/HTMLTableElement)
    libweb_js_wrapper_dep(HTML/HTMLTableRowElement)
    libweb_js_wrapper_dep(HTML/HTMLTableSectionElement)
    libweb_js_wrapper_dep(HTML/HTMLTemplateElement)
    libweb_js_wrapper_dep(HTML/HTMLTextAreaElement)
    libweb_js_wrapper_dep(HTML/HTMLTimeElement)
    libweb_js_wrapper_dep(HTML/HTMLTitleElement)
    libweb_js_wrapper_dep(HTML/HTMLTrackElement)
    libweb_js_wrapper_dep(HTML/HTMLUListElement)
    libweb_js_wrapper_dep(HTML/HTMLUnknownElement)
    libweb_js_wrapper_dep(HTML/HTMLVideoElement)
    libweb_js_wrapper_dep(HTML/ImageData)
    libweb_js_wrapper_dep(HTML/MessageEvent)
    libweb_js_wrapper_dep(HTML/SubmitEvent)
    libweb_js_wrapper_dep(HTML/WebSocket)
    libweb_js_wrapper_dep(HighResolutionTime/Performance)
    libweb_js_wrapper_dep(NavigationTiming/PerformanceTiming)
    libweb_js_wrapper_dep(SVG/SVGElement)
    libweb_js_wrapper_dep(SVG/SVGGeometryElement)
    libweb_js_wrapper_dep(SVG/SVGGraphicsElement)
    libweb_js_wrapper_dep(SVG/SVGPathElement)
    libweb_js_wrapper_dep(SVG/SVGSVGElement)
    libweb_js_wrapper_dep(UIEvents/MouseEvent)
    libweb_js_wrapper_dep(UIEvents/UIEvent)
    libweb_js_wrapper_dep(XHR/ProgressEvent)
    libweb_js_wrapper_dep(XHR/XMLHttpRequest)
    libweb_js_wrapper_dep(XHR/XMLHttpRequestEventTarget)


    if (NOT ENABLE_OSS_FUZZ AND NOT ENABLE_FUZZER_SANITIZER)
        enable_testing()
        add_library(LagomTest $<TARGET_OBJECTS:LagomCore> ${LAGOM_TEST_SOURCES})
        add_executable(TestApp TestApp.cpp)
        target_link_libraries(TestApp Lagom)
        target_link_libraries(TestApp stdc++)

        add_executable(TestJson TestJson.cpp)
        target_link_libraries(TestJson Lagom)
        target_link_libraries(TestJson stdc++)

        add_executable(adjtime_lagom ../../Userland/Utilities/adjtime.cpp)
        set_target_properties(adjtime_lagom PROPERTIES OUTPUT_NAME adjtime)
        target_link_libraries(adjtime_lagom Lagom)

        add_executable(js_lagom ../../Userland/Utilities/js.cpp)
        set_target_properties(js_lagom PROPERTIES OUTPUT_NAME js)
        target_link_libraries(js_lagom Lagom)
        target_link_libraries(js_lagom stdc++)
        target_link_libraries(js_lagom pthread)

        add_executable(ntpquery_lagom ../../Userland/Utilities/ntpquery.cpp)
        set_target_properties(ntpquery_lagom PROPERTIES OUTPUT_NAME ntpquery)
        target_link_libraries(ntpquery_lagom Lagom)

        add_executable(test-js_lagom ../../Tests/LibJS/test-js.cpp)
        set_target_properties(test-js_lagom PROPERTIES OUTPUT_NAME test-js)
        target_link_libraries(test-js_lagom Lagom)
        target_link_libraries(test-js_lagom stdc++)
        target_link_libraries(test-js_lagom pthread)
        add_test(
            NAME JS
            COMMAND test-js_lagom --show-progress=false
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        add_executable(test-crypto_lagom ../../Userland/Utilities/test-crypto.cpp)
        set_target_properties(test-crypto_lagom PROPERTIES OUTPUT_NAME test-crypto)
        target_link_libraries(test-crypto_lagom Lagom)
        target_link_libraries(test-crypto_lagom stdc++)
        add_test(
            NAME Crypto
            COMMAND test-crypto_lagom test -t -s google.com --ca-certs-file ../../Base/etc/ca_certs.ini
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )

        add_executable(disasm_lagom ../../Userland/Utilities/disasm.cpp)
        set_target_properties(disasm_lagom PROPERTIES OUTPUT_NAME disasm)
        target_link_libraries(disasm_lagom Lagom)
        target_link_libraries(disasm_lagom stdc++)

        add_executable(shell_lagom ../../Userland/Shell/main.cpp)
        set_target_properties(shell_lagom PROPERTIES OUTPUT_NAME shell)
        target_link_libraries(shell_lagom Lagom)
        target_link_libraries(shell_lagom stdc++)
        target_link_libraries(shell_lagom pthread)

        add_executable(gml-format_lagom ../../Userland/Utilities/gml-format.cpp)
        set_target_properties(gml-format_lagom PROPERTIES OUTPUT_NAME gml-format)
        target_link_libraries(gml-format_lagom Lagom)
        target_link_libraries(gml-format_lagom stdc++)

        add_executable(sql_lagom ../../Userland/Utilities/sql.cpp)
        set_target_properties(sql_lagom PROPERTIES OUTPUT_NAME sql)
        target_link_libraries(sql_lagom Lagom)
        target_link_libraries(sql_lagom stdc++)

        foreach(TEST_PATH ${SHELL_TESTS})
            get_filename_component(TEST_NAME ${TEST_PATH} NAME_WE)
            add_test(
                NAME "Shell-${TEST_NAME}"
                COMMAND shell_lagom --skip-shellrc "${TEST_PATH}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../Userland/Shell/Tests
            )
            set_tests_properties("Shell-${TEST_NAME}" PROPERTIES
                TIMEOUT 10
                FAIL_REGULAR_EXPRESSION "FAIL"
                PASS_REGULAR_EXPRESSION "PASS"
            )
        endforeach()

        foreach(source ${AK_TEST_SOURCES})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                # FIXME: Only TestJSON needs this property
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../Tests/AK
            )
        endforeach()

        foreach(source ${LIBREGEX_TESTS})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LAGOM_REGEX_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()

        foreach(source ${LIBCOMPRESS_TESTS})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBCOMPRESS_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom Lagom LagomTest)
            add_test(
                    NAME ${name}_lagom
                    COMMAND ${name}_lagom
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()

        foreach(source ${LIBSQL_TEST_SOURCES})
            get_filename_component(name ${source} NAME_WE)
            add_executable(${name}_lagom ${source} ${LIBSQL_SOURCES} ${LIBTEST_MAIN})
            target_link_libraries(${name}_lagom LagomTest)
            add_test(
                NAME ${name}_lagom
                COMMAND ${name}_lagom
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
        endforeach()
    endif()
endif()

if (ENABLE_FUZZER_SANITIZER OR ENABLE_OSS_FUZZ)
    add_subdirectory(Fuzzers)
endif()
