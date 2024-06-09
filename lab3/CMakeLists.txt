cmake_minimum_required(VERSION 3.0)
project(TableLib)
add_library(TableLib func.c)
set_target_properties(TableLib PROPERTIES LINKER_LANGUAGE C)

