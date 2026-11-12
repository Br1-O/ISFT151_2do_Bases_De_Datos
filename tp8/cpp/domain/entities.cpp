#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Libro {
private:
    std::string titulo;
    std::string autor;

public:
    Libro(std::string titulo, std::string autor)
        : titulo(std::move(titulo)), autor(std::move(autor)) {}

    const std::string& getTitulo() const { return titulo; }
    const std::string& getAutor() const { return autor; }

    void mostrar() const {
        std::cout << "Titulo: " << titulo << " | Autor: " << autor << '\n';
    }
};

class Biblioteca {
private:
    std::string nombre;
    std::vector<std::shared_ptr<Libro>> libros;

public:
    explicit Biblioteca(std::string nombre) : nombre(std::move(nombre)) {}

    const std::string& getNombre() const { return nombre; }
    const std::vector<std::shared_ptr<Libro>>& getLibros() const { return libros; }

    void agregarLibro(const std::shared_ptr<Libro>& libro) {
        libros.push_back(libro);
    }

    void mostrar() const {
        std::cout << "\nBiblioteca: " << nombre << '\n';
        for (const auto& libro : libros) {
            libro->mostrar();
        }
    }
};