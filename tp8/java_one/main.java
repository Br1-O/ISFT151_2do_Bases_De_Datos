import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

// ==========================================
// 1. Dominio
// ==========================================
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

// ==========================================
// 2. Interfaz e Implementación del Mapper
// ==========================================
interface ILibroMapper {
    String aLinea(Libro libro);
    Libro desdeLinea(String linea);
}

class LibroTextMapper implements ILibroMapper {
    @Override
    public String aLinea(Libro libro) {
        return "LIBRO\t" + libro.getTitulo() + "\t" + libro.getAutor();
    }

    @Override
    public Libro desdeLinea(String linea) {
        String[] campos = linea.split("\t", -1);
        if (campos.length != 3 || !campos[0].equals("LIBRO") || campos[1].isEmpty() || campos[2].isEmpty()) {
            throw new IllegalArgumentException("Registro de libro invalido");
        }
        return new Libro(campos[1], campos[2]);
    }
}

// ==========================================
// 3. Interfaz e Implementación del Repository
// ==========================================
interface IBibliotecaRepository {
    void guardar(Biblioteca biblioteca) throws IOException;
    Biblioteca cargar() throws IOException;
}

class BibliotecaArchivoRepository implements IBibliotecaRepository {
    private final Path rutaArchivo;
    private final ILibroMapper mapper;

    public BibliotecaArchivoRepository(String rutaArchivo, ILibroMapper mapper) {
        this.rutaArchivo = Path.of(rutaArchivo);
        this.mapper = mapper;
    }

    @Override
    public void guardar(Biblioteca biblioteca) throws IOException {
        List<String> lineas = new ArrayList<>();
        lineas.add("BIBLIOTECA\t" + biblioteca.getNombre());

        for (Libro libro : biblioteca.getLibros()) {
            lineas.add(mapper.aLinea(libro));
        }

        Files.write(rutaArchivo, lineas);
    }

    @Override
    public Biblioteca cargar() throws IOException {
        List<String> lineas = Files.readAllLines(rutaArchivo);
        if (lineas.isEmpty()) {
            throw new IllegalArgumentException("Archivo vacio");
        }

        String[] cabecera = lineas.get(0).split("\t", -1);
        if (cabecera.length != 2 || !cabecera[0].equals("BIBLIOTECA") || cabecera[1].isEmpty()) {
            throw new IllegalArgumentException("Registro de biblioteca invalido");
        }

        Biblioteca biblioteca = new Biblioteca(cabecera[1]);
        for (int i = 1; i < lineas.size(); i++) {
            if (lineas.get(i).isEmpty()) continue;
            biblioteca.agregarLibro(mapper.desdeLinea(lineas.get(i)));
        }

        return biblioteca;
    }
}

// ==========================================
// 4. Punto de Entrada (Main)
// ==========================================
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

            // Inyección mediante interfaces
            ILibroMapper mapper = new LibroTextMapper();
            IBibliotecaRepository repository = new BibliotecaArchivoRepository("biblioteca.txt", mapper);

            repository.guardar(biblioteca);

            Biblioteca recuperada = repository.cargar();

            System.out.println("\nESTRUCTURA RECUPERADA");
            recuperada.mostrar();

        } catch (IOException | IllegalArgumentException error) {
            System.err.println("Error: " + error.getMessage());
        }
    }
}