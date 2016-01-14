#parse("C File Header.h")

#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}

${NAMESPACES_OPEN}

//! My class
/*!
    My class description
*/
class ${NAME}
{
public:
    ${NAME}() = default;
    ${NAME}(const ${NAME}&) = default;
    ${NAME}(${NAME}&&) = default;
    ~${NAME}() = default;

    ${NAME}& operator=(const ${NAME}&) = default;
    ${NAME}& operator=(${NAME}&&) = default;
}

${NAMESPACES_CLOSE}

#[[#endif]]# //${INCLUDE_GUARD}
