#ifndef ILIBRO_MAPPER_HPP
#define ILIBRO_MAPPER_HPP

#include "../domain/entities.hpp"

class ILibroMapper {
public:
    virtual ~ILibroMapper() = default;
    virtual std::string aLinea(const Libro& libro) const = 0;
    virtual std::shared_ptr<Libro> desdeLinea(const std::string& linea) const = 0;
};

#endif