_monitor ParcMon{

  private static final int NB_SEAT = 4;
  
  _condvar clReady;
  _condvar carReady;
  _condvar clWait;

  private static int carEmpty = 0;
  private static boolean firstLoop = true;

  _proc void Client(int id){
    System.out.println("Client " +id+" en attente.");
    if(carEmpty==(NB_SEAT)){
      _wait(carReady);
    }
    carEmpty++;
    System.out.println("Client " +id+" dans la voiture.");
    if(firstLoop)
      _signal(clReady);
    else
      _wait(clReady);
    System.out.println("Client " +id+" tour fini.");
    if(firstLoop)
      _signal(clWait);
    else
      _wait(clWait);
    _signal(carReady);
  }
  
  _proc void Voiture(){
      if(firstLoop){
        while(carEmpty<=(NB_SEAT-1)){
          _wait(clReady);
        }
        System.out.println("Debut du tour.");
        for(int i = 0; i<5; i++){
          System.out.println("Tour "+(i+1));
        }
      }
      else {
        if(_empty(carReady) && _empty(clWait) && _empty(clReady))
          _wait(carReady);
        System.out.println("Debut du tour.");
        for(int i = 0; i<5; i++){
          System.out.println("Tour "+(i+1));
        }
        for(int i =0; i< NB_SEAT; i++){
          _signal(clReady);
        }
      }
      if(firstLoop){
        for(int i = 0; i<NB_SEAT; i++){
          _wait(clWait);
        }
        System.out.println("Fin du tour");
      }
      else{
        System.out.println("Fin du tour");
        for(int i = 0; i<NB_SEAT; i++){
          _signal(clWait);
        }
      }
      carEmpty = 0;
      firstLoop = false;
  }
}
