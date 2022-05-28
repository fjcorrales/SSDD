// Clase que implementa cerrojos para secuenciar las escrituras
// en la copia primaria
// NO MODIFICAR
package datanode;

import java.util.HashMap;
import java.util.concurrent.locks.ReentrantLock;

public class LockManager {
    private HashMap<String, Lock> mapLock;

    public LockManager() {
	    mapLock = new HashMap<String, Lock>();
    }
    public class Lock {
        private LockManager lckMgr;
        private int count;
	private String name;
	private ReentrantLock rLock;
	Lock(LockManager lm, String lName) {
	    count=1;
	    lckMgr=lm;
	    name=lName;
            rLock=new ReentrantLock(true);
	}
	public void lock() {
	    rLock.lock();
	}
	public void unlock() {
	    rLock.unlock();
            lckMgr.delUser(this);
	}
    }
    public synchronized Lock openLock(String name) {
	Lock lock;
        if ((lock = mapLock.get(name))==null){
            lock=new Lock(this, name);
            mapLock.put(name, lock);
	}
	else {
            lock.count++;
	}
	return lock;
    }
    private synchronized void delUser(Lock lock) {
	    if (--lock.count==0)
		mapLock.remove(lock.name);
    }
}

