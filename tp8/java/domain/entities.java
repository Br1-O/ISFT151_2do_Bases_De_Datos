import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

class Libro {
    private final String titulo;
    private final String autor;

    public Libro(String titulo, String autor) {
        this.titulo = titulo;
        this.autor = autor;
    }

    public String getTitulo() { return titulo; }
    public String getAutor() { return autor; }

    public void mostrar() {
        System.out.println("Titulo: " + titulo + " | Autor: " + autor);
    }
}

class Biblioteca {
    private final String nombre;
    private final List<Libro> libros = new ArrayList<>();

    public Biblioteca(String nombre) {
        this.nombre = nombre;
    }

    public String getNombre() { return nombre; }
    public List<Libro> getLibros() { return Collections.unmodifiableList(libros); }

    public void agregarLibro(Libro libro) {
        libros.add(libro);
    }

    public void mostrar() {
        System.out.println("\nBiblioteca: " + nombre);
        for (Libro libro : libros) {
            libro.mostrar();
        }
    }
}