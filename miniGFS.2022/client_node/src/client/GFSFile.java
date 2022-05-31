
// Clase de cliente que proporciona los métodos para acceder a los ficheros.
// Corresponde al API ofrecida a las aplicaciones 

package client;
import java.rmi.*;
import interfaces.*;
import java.util.*;

public class GFSFile {

    private File file;
    private String port,host;
    private Master m;
    private int pointer;
    private int chunkSize;


    public GFSFile(String fileName){
    	pointer = 0;
    	String p = "BROKER_PORT";
    	String h = "BROKER_HOST";
    	port = System.getenv(p);
	host = System.getenv(h);
	chunkSize = Integer.parseInt(java.lang.System.getenv("CHUNKSIZE"));
	try{
		m = (Master) Naming.lookup("//" + host + ":" + port + "/GFS_master");
    		file = m.lookup(fileName);
    	} catch (Exception e){
		System.out.println("Error en el constructor GFSFile");
		e.printStackTrace();
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
		resultado = file.getNumberOfChunks() * chunkSize;
	} catch (Exception e){
		System.out.println("Error en length");
	}
        return resultado;
    }
    // lee de la posición actual del fichero la cantidad de datos pedida;
    // devuelve cantidad realmente leída, 0 si EOF;
    // actualiza la posición de acceso al fichero
    public int read(byte [] buf) throws RemoteException {
       int leido = 0;
       int nChunks=buf.length/chunkSize;
       ArrayList<Chunk> listaChunks;
       ArrayList<DataNode> listaDataNodes;
       Chunk sigChunk;
       byte[] bLeidos;
       int ultEsc = 0;
       try{
		listaChunks = (ArrayList<Chunk>) file.getChunkDescriptors(pointer/chunkSize,nChunks);		
		for(int i=0;i<listaChunks.size();i++){
			sigChunk = listaChunks.get(i);
			if(sigChunk != null){
				bLeidos = sigChunk.getChunkDataNodes().get(0).readChunk(sigChunk.getChunkName());
				for(int j = 0;j<chunkSize;j++){
					buf[j+ultEsc] = bLeidos[j];
				}
				ultEsc = ultEsc + chunkSize;
			} else {
				for(int j=ultEsc;j<ultEsc+chunkSize;j++){
					buf[j]=0;
				}
				ultEsc = ultEsc + chunkSize;
			}
			leido++;
		}
       } catch(Exception e){
		System.out.println("Error en el read del cliente");
		leido = 0;
       }
       pointer = pointer + leido*chunkSize;
        return leido*chunkSize;
    }
    // escribe en la posición actual del fichero los datos especificados;
    // devuelve falso si se ha producido un error en writeChunk;
    // actualiza la posición de acceso al fichero
    public boolean write(byte [] buf) throws RemoteException {
       int nChunks=buf.length/chunkSize;
       List<Chunk> listaChunks = file.allocateChunkDescriptors(pointer/chunkSize,nChunks);
       for(int i=0;i<listaChunks.size();i++){
		List<DataNode> listaDataNodes = listaChunks.get(i).getChunkDataNodes();
		DataNode primero = listaDataNodes.remove(0);
		byte[] buffer = Arrays.copyOfRange(buf,i*chunkSize,(i+1)*chunkSize);
		primero.writeChunk(listaDataNodes,listaChunks.get(i).getChunkName(),buffer);
	}
	pointer = pointer+buf.length;
        return true;
    }
}
