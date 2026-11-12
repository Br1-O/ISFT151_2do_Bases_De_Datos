class LibroTextMapper {
    public static String aLinea(Libro libro) {
        return "LIBRO\t" + libro.getTitulo() + "\t" + libro.getAutor();
    }

    public static Libro desdeLinea(String linea) {
        String[] campos = linea.split("\t", -1);
        if (campos.length != 3 || !campos[0].equals("LIBRO") || campos[1].isEmpty() || campos[2].isEmpty()) {
            throw new IllegalArgumentException("Registro de libro invalido");
        }
        return new Libro(campos[1], campos[2]);
    }
}