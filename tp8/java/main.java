import domain.Biblioteca;
import domain.Libro;
import repo.BibliotecaRepository;

import java.io.IOException;

public class Main {
    public static void main(String[] args) {
        try {
            Biblioteca biblioteca = new Biblioteca("Biblioteca Popular");

            Libro libro1 = new Libro("El Aleph", "Jorge Luis Borges");
            Libro libro2 = new Libro("Rayuela", "Julio Cortazar");

            biblioteca.agregarLibro(libro1);
            biblioteca.agregarLibro(libro2);

            System.out.println("ESTRUCTURA ORIGINAL");
            biblioteca.mostrar();

            BibliotecaRepository repository = new BibliotecaRepository("biblioteca.txt");
            repository.guardar(biblioteca);

            Biblioteca recuperada = repository.cargar();

            System.out.println("\nESTRUCTURA RECUPERADA");
            recuperada.mostrar();

        } catch (IOException | IllegalArgumentException error) {
            System.err.println("Error: " + error.getMessage());
        }
    }
}