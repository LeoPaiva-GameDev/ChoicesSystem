// Code by Leonardo Paiva


#include "NonPlayerCharacter.h"

// Sets default values
ANonPlayerCharacter::ANonPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName ("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("AudioComponent"));
	//AudioComponent->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void ANonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}




