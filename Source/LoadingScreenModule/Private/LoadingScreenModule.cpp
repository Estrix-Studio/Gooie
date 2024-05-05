#include "LoadingScreenModule.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "MoviePlayer.h"

struct FLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	FLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		FSlateBrush::AddReferencedObjects(Collector);
	}

	virtual FString GetReferencerName(void) const
	{
		return "FLoadingScreenBrush";
	}
};

class SIngameLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SIngameLoadingScreen) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs)
	{
		//"/Game/FirstPerson/UI/Assets/LoadingScreenBG.LoadingScreenBG"
		static const FName LoadingScreenName(TEXT("/Game/UI/Textures/LoadingScreenBG.LoadingScreenBG"));
		LoadingScreenBrush = MakeShareable(new FLoadingScreenBrush(LoadingScreenName, FVector2D(1920, 1080)));

		ChildSlot
			[
				SNew(SOverlay)
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SImage)
							.Image(LoadingScreenBrush.Get())
					]
					+ SOverlay::Slot()
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					[
						SNew(SSafeZone)
							.VAlign(VAlign_Bottom)
							.HAlign(HAlign_Right)
							.Padding(10.0f)
							.IsTitleSafe(true)
							[
								SNew(SThrobber)
									.Visibility(this, &SIngameLoadingScreen::GetLoadIndicatorVisibility)
							]
					]
			];
	}

private:
	EVisibility GetLoadIndicatorVisibility() const
	{
		return EVisibility::Visible;
	}

	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};


class FLoadingScreenModule : public ILoadingScreenModule
{
public:

	virtual void StartupModule() override
	{
		UE_LOG(LogTemp, Warning, TEXT("FLoadingScreenModule::StartupModule"));
		LoadObject<UObject>(NULL, TEXT("/Game/UI/Textures/LoadingScreenBG.LoadingScreenBG"));
	}

	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void StartInGameLoadingScreen() override
	{
		UE_LOG(LogTemp, Warning, TEXT("FLoadingScreenModule::StartInGameLoadingScreen"));
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.MinimumLoadingScreenDisplayTime = 1.0f;
		LoadingScreen.WidgetLoadingScreen = SNew(SIngameLoadingScreen);

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}


};
IMPLEMENT_GAME_MODULE(FLoadingScreenModule, LoadingScreenModule)
