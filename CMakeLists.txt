cmake_minimum_required(VERSION 3.20)

# プロジェクトの設定
project(YamanekoEngineProject CXX)
set(TARGET_NAME ${PROJECT_NAME})
add_executable(${PROJECT_NAME})

# C++の環境設定
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ターゲットに直接 C++23 を要求
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# MSVCの場合、ソースコードの文字エンコーディングをUTF-8に設定
if(MSVC)
  add_compile_options("/utf-8")
endif()

# 依存パッケージの検索
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(assimp REQUIRED)
find_package(OpenGL REQUIRED)

# ターゲットにライブラリをリンク
target_link_libraries(${PROJECT_NAME} PRIVATE
    yamaneko_engine
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    glad::glad
    imgui::imgui
    assimp::assimp
    OpenGL::GL
)

# ソースコードディレクトリの追加
add_subdirectory(engine)
add_subdirectory(src)

# vcpkg でインストールされた全パッケージのライセンスを収集
if(DEFINED VCPKG_INSTALLED_DIR AND DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_SHARE_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share")
    file(GLOB VCPKG_COPYRIGHT_FILES "${VCPKG_SHARE_DIR}/*/copyright")

    foreach(COPYRIGHT_FILE ${VCPKG_COPYRIGHT_FILES})
        get_filename_component(PORT_DIR "${COPYRIGHT_FILE}" DIRECTORY)
        get_filename_component(PORT_NAME "${PORT_DIR}" NAME)

        if(PORT_NAME MATCHES "^vcpkg-")
            continue()
        endif()

        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/licenses"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${COPYRIGHT_FILE}"
                "$<TARGET_FILE_DIR:${TARGET_NAME}>/licenses/${PORT_NAME}-LICENSE.txt"
        )
    endforeach()
endif()

# アセットディレクトリのコピー設定
set(ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "$<TARGET_FILE_DIR:${TARGET_NAME}>/assets")

# 注意: assetsディレクトリが存在しない場合、コピーは失敗します。事前にassetsディレクトリを作成してください。
add_custom_command(
    TARGET ${TARGET_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${ASSETS_SRC_DIR}"
            "${ASSETS_DEST_DIR}"
)

