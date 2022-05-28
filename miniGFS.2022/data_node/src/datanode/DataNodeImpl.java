// Implementación de la interfaz de un nodo de datos

package datanode;
import java.util.List;
import java.rmi.*;
import java.rmi.server.*;
import java.io.*;

import interfaces.*;

public class DataNodeImpl extends UnicastRemoteObject implements DataNode {
    public static final long  serialVersionUID=1234567891;

	private String name;
	private Manager man;

    public DataNodeImpl(Manager m, String n) throws RemoteException {
    	name = n;
    	man = m;
    	man.addDataNode(this);
    }
    // nombre del nodo
    public String getName() throws RemoteException {
        return name;
    }
    // lee el fichero que contiene un chunk
    public byte [] readChunk(String chunkName) throws RemoteException {
        return null;
    }
    // escribe en un fichero local el contenido del chunk;
    // si la lista de nodos pasada como parámetro no esta vacía,
    // propaga la escritura a los nodos de datos de la lista
    public boolean writeChunk(List <DataNode> nodes, String chunkName, byte [] buffer) throws RemoteException {
    	boolean resultado = false;
    	if(nodes.size() == 0){
    		try {
			FileOutputStream fichero =  new FileOutputStream(chunkName);
			fichero.write(buffer);
			resultado = true;
		} catch (Exception e){
			System.out.println("Error en la escritura de un chunk");
		}
    	}
        return resultado;
    }
}
