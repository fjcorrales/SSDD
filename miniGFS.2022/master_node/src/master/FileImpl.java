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
	public int tam = 0;

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
		return tam;
	}

	// obtiene información de ubicación de los chunks especificados del fichero
	public List <Chunk> getChunkDescriptors(int nchunk, int size) throws RemoteException {
		List<Chunk> result = new ArrayList<Chunk>();

		for(int i = nchunk ; i<nchunk+size ; i++){

			if(mapaChunks.containsKey(i)){
				result.add(mapaChunks.get(i));
			}else if(!mapaChunks.containsKey(i) && i<tam){
				result.add(null);
			}
		}
		return result;
	}

	// reserva información de ubicación para los chunks especificados del fichero
	public List <Chunk> allocateChunkDescriptors(int nchunk, int size) throws RemoteException {
		List<Chunk> result = new ArrayList<Chunk>();

		//For para cargar "todo" el fichero en el mapa de chunks para evitar el fallo de "tamaño"
		for(int i = nchunk ; i<nchunk ; i++){
			if(!mapaChunks.containsKey(i)){
				List<DataNode> dataNode = new ArrayList<DataNode>();
				for(int j = 0 ; j<repFact ; j++){
					DataNode nuevo = manager.selectDataNode();
					dataNode.add(nuevo);
				}
				ChunkImpl newChunk = new ChunkImpl(dataNode);
				mapaChunks.put(i, newChunk);
			}
		}

		for(int i = nchunk ; i<nchunk + size ; i++){
			if(!mapaChunks.containsKey(i)){							//Si no existen chunks, creo el chunk y lo añado al mapa
				List<DataNode> dataNode = new ArrayList<DataNode>();
				//for para hacer nueva lista de nodos por replica
				for(int j = 0 ; j<repFact ; j++){
					DataNode nuevo = manager.selectDataNode();
					dataNode.add(nuevo);
				}
				ChunkImpl newChunk = new ChunkImpl(dataNode);
				mapaChunks.put(i, newChunk);
				result.add(newChunk);
			}else{										//Si existe el chunk, lo añado a la lista resultado
				result.add(mapaChunks.get(i));
			}
		}
		if(tam<(nchunk+size)){
			tam = nchunk+size;
		}
		return result;
	}
}
