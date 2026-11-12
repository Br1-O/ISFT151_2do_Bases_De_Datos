import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;

class BibliotecaRepository {
    private final Path rutaArchivo;

    public BibliotecaRepository(String rutaArchivo) {
        this.rutaArchivo = Path.of(rutaArchivo);
    }

    public void guardar(Biblioteca biblioteca) throws IOException {
        List<String> lineas = new ArrayList<>();
        lineas.add("BIBLIOTECA\t" + biblioteca.getNombre());

        for (Libro libro : biblioteca.getLibros()) {
            lineas.add(LibroTextMapper.aLinea(libro));
        }

        Files.write(rutaArchivo, lineas);
    }

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
            biblioteca.agregarLibro(LibroTextMapper.desdeLinea(lineas.get(i)));
        }

        return biblioteca;
    }
}