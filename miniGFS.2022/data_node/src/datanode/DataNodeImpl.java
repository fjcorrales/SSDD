// Implementación de la interfaz de un nodo de datos

package datanode;
import java.util.List;
import java.rmi.*;
import java.rmi.server.*;
import java.io.*;
import java.nio.file.Files;

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
	int chunkSize = Integer.parseInt(java.lang.System.getenv("CHUNKSIZE"));
	byte[] chunk = new byte[64000000];
	try{
		java.io.File file = new java.io.File("./"+name+"/"+chunkName);
		chunk = Files.readAllBytes(file.toPath());
	} catch(Exception e) {
		System.out.println("Error en la lectura del chunk");
		return null;
	}
        return chunk;
    }
    // escribe en un fichero local el contenido del chunk;
    // si la lista de nodos pasada como parámetro no esta vacía,
    // propaga la escritura a los nodos de datos de la lista
    public boolean writeChunk(List <DataNode> nodes, String chunkName, byte [] buffer) throws RemoteException {
    	boolean resultado = false;
    	FileOutputStream fichero;
    	if(nodes == null){
    		try {
			java.io.File f = new java.io.File("./"+name+"/"+chunkName);
			if(!f.exists()){
				f.createNewFile();
			}
			fichero =  new FileOutputStream(f);
			fichero.write(buffer);
			resultado = true;
			fichero.close();
		} catch (Exception e){
			System.out.println("Error en la escritura de un chunk");
			resultado=false;
		}
    	} else {
		LockManager lck = new LockManager();
		LockManager.Lock l;
		java.io.File f = new java.io.File("./"+name+"/"+chunkName);
		if(!f.getParentFile().exists()){
			f.getParentFile().mkdirs();
		}
		if(!f.exists()){
			try{
				f.createNewFile();
			} catch(Exception e){
				System.out.println("Error en la escritura secundaria de un chunk");
			}
		}
		try{
			fichero = new FileOutputStream(f);
			fichero.write(buffer);
			fichero.close();
		} catch(Exception e){
			System.out.println("Error en la escritura en nodo secundario de un chunk");
		}
		for(DataNode node:nodes){
			(l = lck.openLock(chunkName)).lock();
			node.writeChunk(null,chunkName,buffer);
			l.unlock();
		}
		resultado=true;
    	}
        return resultado;
    }
}
