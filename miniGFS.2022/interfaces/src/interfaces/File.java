// Interfaz remota para el acceso a la información de ubicación de un fichero
// NO MODIFICAR

package interfaces;
import java.rmi.*;
import java.util.List;

public interface File extends Remote {
    // nº de chunks del fichero
    public int getNumberOfChunks() throws RemoteException;
    // obtiene información de ubicación de los chunks especificados del fichero
    public List <Chunk> getChunkDescriptors(int nchunk, int size) throws RemoteException;
    // reserva información de ubicación para los chunks especificados del fichero
    public List <Chunk> allocateChunkDescriptors(int nchunk, int size) throws RemoteException;
}
