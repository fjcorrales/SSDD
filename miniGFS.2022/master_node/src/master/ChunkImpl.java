// Clase que implementa la interfaz Chunk. No es una clase remota.
// NO MODIFICAR
package master;
import java.util.UUID;
import java.util.List;
import java.io.Serializable;
import interfaces.*;

public class ChunkImpl implements Chunk, Serializable {
    public static final long  serialVersionUID=1234567891;
    String name;
    List <DataNode> DataNodesList;
    public ChunkImpl(List <DataNode> dnl) {
        name = UUID.randomUUID().toString();
        DataNodesList=dnl;
    }
    // nombre del chunk (UUID)
    public String getChunkName() {
        return name;
    }
    // nodos de datos donde está ubicado el chunk; 1º de lista: copia primaria
    public List <DataNode> getChunkDataNodes() {
        return DataNodesList;
    }
}
