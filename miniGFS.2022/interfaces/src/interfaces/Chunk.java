// Interfaz para el acceso a la información de un chunk
// Nótese que no se trata de una interfaz remota
// NO MODIFICAR

package interfaces;
import java.util.List;

public interface Chunk {
    // nombre del chunk (UUID)
    public String getChunkName();
    // nodos de datos donde está ubicado el chunk; 1º de lista: copia primaria
    public List <DataNode> getChunkDataNodes();
}
