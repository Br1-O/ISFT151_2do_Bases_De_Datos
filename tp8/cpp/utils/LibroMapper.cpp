#include "ILibroMapper.hpp"
#include <sstream>
#include <stdexcept>

class LibroTextMapper : public ILibroMapper {
public:
    std::string aLinea(const Libro& libro) const override {
        return "LIBRO\t" + libro.getTitulo() + "\t" + libro.getAutor();
    }

    std::shared_ptr<Libro> desdeLinea(const std::string& linea) const override {
        std::istringstream entrada(linea);
        std::string tipo, titulo, autor;

        std::getline(entrada, tipo, '\t');
        std::getline(entrada, titulo, '\t');
        std::getline(entrada, autor);

        if (tipo != "LIBRO" || titulo.empty() || autor.empty()) {
            throw std::runtime_error("Registro de libro invalido");
        }
        return std::make_shared<Libro>(titulo, autor);
    }
};