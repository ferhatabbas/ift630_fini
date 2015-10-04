_monitor BarbMon{

  private static final int NB_SEAT = 4;
  
  _condvar clReady;
  _condvar clWait;
  _condvar clExit;
  _condvar cutDone;
  _condvar seatReady;
  
  _condvar barbReady;
  _var boolean barb_ready = true;

  private static int seat = NB_SEAT;
  private static boolean unsleep = false;
  
  _proc void Barber(){
    if(seat == NB_SEAT){
      System.out.println("Barber sleeping.");
      unsleep = true;
      _wait(clReady);
    }
    if(unsleep){
      System.out.println("Barber unsleep.");
      unsleep = false;
    }
    seat++;
    _wait(seatReady);
    _wait(cutDone);
    _wait(clExit);
  }

  _proc void NextClient(){
    _signal(barbReady);
  }

  _proc void Client(int id){
    if(seat>0){
      seat--;
      System.out.println("Client "+id+" ready");
      if(!barb_ready){
        System.out.println("Client "+id+" waiting");
        _wait(barbReady);
        System.out.println("Client "+id+" selected");
      }
      if(barb_ready){
        barb_ready=false;
        _signal(clReady);
        System.out.println("Seat ready for client "+id);
        _signal(seatReady);
        System.out.println("Client "+id+" cutting hair");
        _signal(cutDone);
        System.out.println("Client "+id+" exit");
        _signal(clExit);
        barb_ready = true;
        _signal(barbReady);
      }
    }else{
      System.out.println("Drop client "+id);
    }
  }
}
