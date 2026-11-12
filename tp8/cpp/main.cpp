#include "domain/entities.cpp"
#include "utils/LibroMapper.cpp"
#include "repo/BibliotecaRepository.cpp"

int main() {
    try {
        Biblioteca biblioteca("Biblioteca Popular");

        auto libro1 = std::make_shared<Libro>("El Aleph", "Jorge Luis Borges");
        auto libro2 = std::make_shared<Libro>("Rayuela", "Julio Cortazar");

        biblioteca.agregarLibro(libro1);
        biblioteca.agregarLibro(libro2);

        std::cout << "ESTRUCTURA ORIGINAL\n";
        biblioteca.mostrar();

        BibliotecaRepository repo("biblioteca.txt");
        repo.guardar(biblioteca);

        auto recuperada = repo.cargar();

        std::cout << "\nESTRUCTURA RECUPERADA\n";
        recuperada->mostrar();
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}