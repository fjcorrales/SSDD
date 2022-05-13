// Implementación de la interfaz remota para el acceso a la información de ubicación de un fichero

package master;
import java.util.*;
import java.rmi.*;
import java.rmi.server.*;
import manager.*;
import interfaces.*;

public class FileImpl extends UnicastRemoteObject implements File {
	public static final long  serialVersionUID=1234567891;
	private Map<Integer, ChunkImpl> mapaChunks;
	private ManagerImpl manager;
	private int repFact;

	/*
		En el constructor, por ahora inicializo el mapa que relaciona chunks con chunksImpl,
		para el tema de la fragmentaciond e ficheros que comentan en el video que se apaña
		bastante bien metiendo esto como un mapa, también me guardo lo que me pasan por valor
	*/
	public FileImpl(ManagerImpl m, int replicaFactor) throws RemoteException {
		mapaChunks = new HashMap<Integer, ChunkImpl>();
		manager = m;
		repFact = replicaFactor;
	}

	// nº de chunks del fichero
	public int getNumberOfChunks() throws RemoteException {
		return 0;
	}

	// obtiene información de ubicación de los chunks especificados del fichero
	public List <Chunk> getChunkDescriptors(int nchunk, int size) throws RemoteException {
		List<Chunk> infoChunks = new ArrayList<Chunk>();					//Lista con los chunks de informacion que se va a devolver
		int aux = nchunk;
//	For para ir escanando el mapa para ver si se encuentran chunks ya creados en ese rango
		for(int i = 0 ; i<size ; i++){
			if(mapaChunks.containsKey(aux)){							//Si hay chunks en ese intervalo, los añado a la lista que voy a devolcer
				infoChunks.add(mapaChunks.get(aux));
			}else{										//si no esta en el mapa, se pone un null
				infoChunks.add(null);
			}
			aux++;
		}
		return infoChunks;
	}

	// reserva información de ubicación para los chunks especificados del fichero
	public List <Chunk> allocateChunkDescriptors(int nchunk, int size) throws RemoteException {
		ArrayList<DataNode> nuevoChunk = new ArrayList<DataNode>();
		ArrayList<Chunk> chunkList = new ArrayList<Chunk>();
		int aux = nchunk;
		for(int i = 0 ; i<size ; i++){
			if(!mapaChunks.containsKey(aux)){
				/*
					Si no existe ese chunk, no tiene info, no está en el mapa, se crea y se añade
				*/
				nuevoChunk.add(manager.selectDataNode());
			}
			aux++;
		}
		for(int i = 0 ; i<repFact ; i++){
			//Por cada rplica que tengamos del fichero creamos un chunk distinto
			ChunkImpl nuevo = new ChunkImpl(nuevoChunk);
			mapaChunks.put(nchunk, nuevo);
		}
		for(Integer key:mapaChunks.keySet()){
			chunkList.add(mapaChunks.get(key));
		}
		return chunkList;
	}
}
