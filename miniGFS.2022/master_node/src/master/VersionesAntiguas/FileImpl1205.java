// Implementación de la interfaz remota para el acceso a la información de ubicación de un fichero

package master;
import java.util.List;
import java.rmi.*;
import java.rmi.server.*;
import manager.*;
import interfaces.*;

public class FileImpl extends UnicastRemoteObject implements File {
    public static final long  serialVersionUID=1234567891;

    public FileImpl(ManagerImpl m, int replicaFactor) throws RemoteException {
    }
    // nº de chunks del fichero
    public int getNumberOfChunks() throws RemoteException {
        return 0;
    }
    // obtiene información de ubicación de los chunks especificados del fichero
    public List <Chunk> getChunkDescriptors(int nchunk, int size) throws RemoteException {
        return null;
    }
    // reserva información de ubicación para los chunks especificados del fichero
    public List <Chunk> allocateChunkDescriptors(int nchunk, int size) throws RemoteException {
        return null;
    }
}
