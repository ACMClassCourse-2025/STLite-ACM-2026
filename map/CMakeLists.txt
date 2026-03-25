include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/corner_data)

function(add_map_case case_name)
  string(REPLACE "." "_" case_suffix "${case_name}")
  set(target_name "map_${case_suffix}")

  add_executable(
    ${target_name}
    ${CMAKE_CURRENT_SOURCE_DIR}/data/${case_name}/code.cpp
  )

  add_test(
    NAME ${target_name}
    COMMAND sh -c
      "${CMAKE_CURRENT_BINARY_DIR}/${target_name} >${CMAKE_CURRENT_BINARY_DIR}/${target_name}.out \
      && diff -u ${CMAKE_CURRENT_SOURCE_DIR}/data/${case_name}/answer.txt ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.out >${CMAKE_CURRENT_BINARY_DIR}/${target_name}.diff"
  )
endfunction()

function(add_map_corner_case case_id)
  set(target_name "map_corner_${case_id}")

  add_executable(
    ${target_name}
    ${CMAKE_CURRENT_SOURCE_DIR}/corner_data/${case_id}.cpp
  )

  add_test(
    NAME ${target_name}
    COMMAND sh -c
      "${CMAKE_CURRENT_BINARY_DIR}/${target_name} >${CMAKE_CURRENT_BINARY_DIR}/${target_name}.out \
      && diff -u ${CMAKE_CURRENT_SOURCE_DIR}/corner_data/${case_id}.ans ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.out >${CMAKE_CURRENT_BINARY_DIR}/${target_name}.diff"
  )
endfunction()

add_map_case(one)
add_map_case(one.memcheck)
add_map_case(two)
add_map_case(two.memcheck)
add_map_case(three)
add_map_case(three.memcheck)
add_map_case(four)
add_map_case(four.memcheck)
add_map_case(five)
add_map_case(five.memcheck)

add_map_corner_case(1)
add_map_corner_case(2)
add_map_corner_case(3)
