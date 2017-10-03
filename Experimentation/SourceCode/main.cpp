int Optymalizacja(void)
{
  Opt o("test");
  o.CzytajDane();
  o.ModyfikujGraf();
  if (o.Optymalizacja()) printf("OK\n");
  else printf("Nic z tego\n");


  return TRUE;

}



int main(int argc, char* argv[])
{
	Optymalizacja();
	return 0;
}

