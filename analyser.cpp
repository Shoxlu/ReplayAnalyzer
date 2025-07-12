#include "analyser.h"
#include "touhou18.h"

Analyzer::Analyzer()
{
	observations.resize(2);
	Sleep(3000);
	observations[Lives] = WatcherInt(&global_ptr->inner.current_lives);//is it useful to watch for lives when the game already tracks that ? Guess it's PoC...
	observations[Bombs] = WatcherInt(&global_ptr->inner.current_bombs);
}

Analyzer::~Analyzer()
{
}

void Analyzer::Update()
{
	size_t n_observations = observations.size();
	for (size_t i = 0; i < n_observations; i++)
	{
		observations[i].Update();
	}
	
	//Weird code coming, don't know yet how to make it suck less

	if (observations[Lives].hasDecreased()) {
		results.n_miss -= observations[Lives].diff;
		printf("Miss !\n");
	}
	else if (observations[Bombs].hasDecreased()) {
		results.n_bombs -= observations[Bombs].diff;
		printf("Bomb !\n");
	}
	
}

void Analyzer::SaveResults(std::string file_name)
{
	//Temporary
	printf("Misses: %d, Bombs used: %d\n", results.n_miss, results.n_bombs);
}
