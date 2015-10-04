_monitor PhiloMon{
  public static final int NB_PROC = 5;

  // Thinking = 0, eating = 1, waiting = 2
  _var int[] state = new int[NB_PROC];
  _condvar vwait[NB_PROC];
  
  _proc void take(int id){
    int before = (id-1)<0?(NB_PROC-1):(id-1);
    int after = (id+1)%NB_PROC;
    state[id]=2;
    if(id%2==0){
      int tmp = before;
      before=after;
      after=tmp;
    }
    //    System.out.println("Before "+before+" state :" +state[before]);
    //    System.out.println("After "+after+" state :" +state[after]);
    if(state[before]==1){
      //System.out.println("wait B, "+before);
      _wait(vwait[before]);
    }
    if(state[after]==1){
      //System.out.println("wait A, "+after);
      _wait(vwait[after]);
    }
    state[id]=1;
  }
  
  _proc void release(int id){
    state[id]=0;
    _signal(vwait[id]);
  }
}
