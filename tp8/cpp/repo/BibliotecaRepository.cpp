#include <fstream>

class BibliotecaRepository {
private:
    std::string rutaArchivo;

public:
    explicit BibliotecaRepository(std::string ruta) : rutaArchivo(std::move(ruta)) {}

    void guardar(const Biblioteca& biblioteca) const {
        std::ofstream salida(rutaArchivo);
        if (!salida) {
            throw std::runtime_error("No se pudo abrir el archivo para escribir");
        }

        salida << "BIBLIOTECA\t" << biblioteca.getNombre() << '\n';
        for (const auto& libro : biblioteca.getLibros()) {
            salida << LibroTextMapper::aLinea(*libro) << '\n';
        }
    }

    std::unique_ptr<Biblioteca> cargar() const {
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
            biblioteca->agregarLibro(LibroTextMapper::desdeLinea(linea));
        }

        return biblioteca;
    }
};