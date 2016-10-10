esempioLettura(string fname)
{
  // check to see if the event class is in the dictionary
  // if it is not load the definition in libEvent.so
  if (!TClassTable::GetDict("Event")) 
    {
      gSystem->Load("libCMOS");
    }
}
