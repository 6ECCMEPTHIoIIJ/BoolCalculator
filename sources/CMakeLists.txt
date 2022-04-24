set(SOURCES "main.cpp"
        "truth_table.cpp"
        "gluing_table.cpp"
        "constituent.cpp"
        "cantor_normal_form.cpp"
        "cantor_perfect_normal_form.cpp"
        "cantor_abbreviated_normal_form.cpp"
        "quine_matrix.cpp"
        "quine_core.cpp"
        "dead_end_cantor_normal_forms.cpp"
        "output.cpp"
        "reverse_polish_notation.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})