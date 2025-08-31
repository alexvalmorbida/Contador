# CMakeList.txt: projeto de CMake para contador, incluir origem e definir
# lógica específica a um projeto aqui.
#
cmake_minimum_required (VERSION 3.8)

# Define o padrão do C++ para C++17 ou C++20.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontra a ferramenta PkgConfig, essencial para localizar bibliotecas no MSYS2 e Linux.
find_package(PkgConfig REQUIRED)

# Usa o PkgConfig para encontrar a biblioteca GTKmm 4.0 e configurar automaticamente
# as variáveis de compilação e de ligação (include dirs e libs).
pkg_check_modules(GTKMM REQUIRED gtkmm-4.0)

project ("contador")

# Adicione a origem ao executável deste projeto.
add_executable (contador "src/main.cpp" "src/Contador.cpp" "src/JanelaContador.cpp")

target_include_directories(contador PUBLIC include)


# Adiciona os diretórios de cabeçalho do GTKmm ao nosso alvo.
# Isto permite que o compilador encontre os ficheiros #include <gtkmm.h>.
target_include_directories(contador PRIVATE ${GTKMM_INCLUDE_DIRS})


# Liga as bibliotecas do GTKmm ao nosso executável.
# Isto garante que o código das funções do GTKmm seja incluído no programa final.
target_link_libraries(contador PRIVATE ${GTKMM_LIBRARIES})

#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET contador PROPERTY CXX_STANDARD 20)
#endif()

# TODO: Adicione testes e instale destinos, se necessário.
