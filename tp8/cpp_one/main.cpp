#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

// ==========================================
// 1. Dominio (Entities)
// ==========================================
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

// ==========================================
// 2. Interfaz e Implementación del Mapper
// ==========================================
class ILibroMapper {
public:
    virtual ~ILibroMapper() = default;
    virtual std::string aLinea(const Libro& libro) const = 0;
    virtual std::shared_ptr<Libro> desdeLinea(const std::string& linea) const = 0;
};

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

// ==========================================
// 3. Interfaz e Implementación del Repository
// ==========================================
class IBibliotecaRepository {
public:
    virtual ~IBibliotecaRepository() = default;
    virtual void guardar(const Biblioteca& biblioteca) const = 0;
    virtual std::unique_ptr<Biblioteca> cargar() const = 0;
};

class BibliotecaArchivoRepository : public IBibliotecaRepository {
private:
    std::string rutaArchivo;
    std::shared_ptr<ILibroMapper> mapper;

public:
    BibliotecaArchivoRepository(std::string ruta, std::shared_ptr<ILibroMapper> mapper)
        : rutaArchivo(std::move(ruta)), mapper(std::move(mapper)) {}

    void guardar(const Biblioteca& biblioteca) const override {
        std::ofstream salida(rutaArchivo);
        if (!salida) {
            throw std::runtime_error("No se pudo abrir el archivo para escribir");
        }

        salida << "BIBLIOTECA\t" << biblioteca.getNombre() << '\n';
        for (const auto& libro : biblioteca.getLibros()) {
            salida << mapper->aLinea(*libro) << '\n';
        }
    }

    std::unique_ptr<Biblioteca> cargar() const override {
        std::ifstream entrada(rutaArchivo);
        if (!entrada) {
            throw std::runtime_error("No se pudo abrir el archivo para leer");
        }

        std::string linea;
        if (!std::getline(entrada, linea)) {
            throw std::runtime_error("Archivo vacio");
        }

        std::istringstream primeraLinea(linea);
        std::string tipo, nombre;
        std::getline(primeraLinea, tipo, '\t');
        std::getline(primeraLinea, nombre);

        if (tipo != "BIBLIOTECA" || nombre.empty()) {
            throw std::runtime_error("Registro de biblioteca invalido");
        }

        auto biblioteca = std::make_unique<Biblioteca>(nombre);
        while (std::getline(entrada, linea)) {
            if (linea.empty()) continue;
            biblioteca->agregarLibro(mapper->desdeLinea(linea));
        }

        return biblioteca;
    }
};

// ==========================================
// 4. Punto de Entrada (Main)
// ==========================================
int main() {
    try {
        Biblioteca biblioteca("Biblioteca Popular");

        auto libro1 = std::make_shared<Libro>("El Aleph", "Jorge Luis Borges");
        auto libro2 = std::make_shared<Libro>("Rayuela", "Julio Cortazar");

        biblioteca.agregarLibro(libro1);
        biblioteca.agregarLibro(libro2);

        std::cout << "ESTRUCTURA ORIGINAL\n";
        biblioteca.mostrar();

        // Inyección mediante interfaces
        std::shared_ptr<ILibroMapper> mapper = std::make_shared<LibroTextMapper>();
        std::unique_ptr<IBibliotecaRepository> repo = 
            std::make_unique<BibliotecaArchivoRepository>("biblioteca.txt", mapper);

        repo->guardar(biblioteca);

        auto recuperada = repo->cargar();

        std::cout << "\nESTRUCTURA RECUPERADA\n";
        recuperada->mostrar();
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}