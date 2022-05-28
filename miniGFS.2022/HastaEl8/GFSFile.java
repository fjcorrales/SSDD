// Clase de cliente que proporciona los métodos para acceder a los ficheros.
// Corresponde al API ofrecida a las aplicaciones 

package client;
import java.rmi.*;
import interfaces.*;

public class GFSFile {

    private File file;
    private String port,host;
    private Master m;
    private int pointer;


    public GFSFile(String fileName){
    	pointer = 0;
    	String p = "GFSPort";
    	String h = "GFSHost";
    	port = java.lang.System.getenv(p);
	host = java.lang.System.getenv(h);
	try{
		m = (Master) Naming.lookup("//" + h + ":" + p + "/GFS_master");
    		file = m.lookup(fileName);
    	} catch (Exception e){
		System.out.println("Error en el constructor de GFSFile");
    	}
    }
    // establece la posición de acceso al fichero
    public void seek(int off) {
    	pointer = off;
    }
    // obtiene la posición de acceso al fichero
    public int getFilePointer() {
        return pointer;
    }
    // obtiene la longitud del fichero en bytes
    public int length() throws RemoteException {
	int resultado=0;
	try{
		resultado = file.getNumberOfChunks() * 64000000;
	} catch (Exception e){
		System.out.println("Error en length");
	}
        return resultado;
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
