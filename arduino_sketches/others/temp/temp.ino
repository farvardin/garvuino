
  
void setup()
{
}
 
void loop()
{
  for(int i=0;i<256;i+=2)
    dacWrite(25,i);
  for(int i=254;i>=0;i-=2)
    dacWrite(25,i);
  for(int i=1;i<75;i+=2)
    dacWrite(25,i);
  for(int i=74;i>0;i-=2)
    dacWrite(25,i);
}
