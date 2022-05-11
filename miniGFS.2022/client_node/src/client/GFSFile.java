// Clase de cliente que proporciona los métodos para acceder a los ficheros.
// Corresponde al API ofrecida a las aplicaciones 

package client;
import java.rmi.*;
import interfaces.*;

public class GFSFile {
    public GFSFile(String fileName){
    }
    // establece la posición de acceso al fichero
    public void seek(int off) {
    }
    // obtiene la posición de acceso al fichero
    public int getFilePointer() {
        return 0;
    }
    // obtiene la longitud del fichero en bytes
    public int length() throws RemoteException {
        return 0;
    }
    // lee de la posición actual del fichero la cantidad de datos pedida;
    // devuelve cantidad realmente leída, 0 si EOF;
    // actualiza la posición de acceso al fichero
    public int read(byte [] buf) throws RemoteException {
        return 0;
    }
    // escribe en la posición actual del fichero los datos especificados;
    // devuelve falso si se ha producido un error en writeChunk;
    // actualiza la posición de acceso al fichero
    public boolean write(byte [] buf) throws RemoteException {
        return true;
    }
}
