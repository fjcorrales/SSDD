// Interfaz remota de un nodo de datos
// NO MODIFICAR

package interfaces;
import java.util.List;
import java.rmi.*;

public interface DataNode extends Remote {
    // nombre del nodo
    public String getName() throws RemoteException;
    // lee el fichero que contiene un chunk
    public byte [] readChunk(String chunkName) throws RemoteException;
    // escribe en un fichero local el contenido del chunk;
    // si la lista de nodos pasada como parámetro no esta vacía,
    // propaga la escritura a los nodos de datos de la lista
    public boolean writeChunk(List <DataNode> nodes, String chunkName, byte [] buffer) throws RemoteException;
}

