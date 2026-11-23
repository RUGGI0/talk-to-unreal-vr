#include "LocalTextQuery.h"

#include "Dom/JsonObject.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

ULocalTextQuery* ULocalTextQuery::SendLocalTextQuery(
    UObject* WorldContextObject,
    const FString& Text
)
{
    ULocalTextQuery* Query = NewObject<ULocalTextQuery>();
    Query->QueryText = Text.TrimStartAndEnd();
    Query->RegisterWithGameInstance(WorldContextObject);
    return Query;
}

void ULocalTextQuery::Activate()
{
    if (QueryText.IsEmpty())
    {
        Fail(TEXT("Inserisci un testo."));
        return;
    }

    const TSharedRef<FJsonObject> Body = MakeShared<FJsonObject>();
    Body->SetStringField(TEXT("text"), QueryText);
    FString Json;
    FJsonSerializer::Serialize(Body, TJsonWriterFactory<>::Create(&Json));

    const FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(TEXT("http://127.0.0.1:8000/query"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
    Request->SetContentAsString(Json);
    Request->SetTimeout(10.0f);
    Request->OnProcessRequestComplete().BindWeakLambda(
        this,
        [this](FHttpRequestPtr, FHttpResponsePtr Response, bool bConnected)
        {
            bReceivedCompletion = true;
            if (!bConnected || !Response.IsValid())
            {
                Fail(TEXT("Server Python non raggiungibile."));
                return;
            }

            const int32 Code = Response->GetResponseCode();
            if (!EHttpResponseCodes::IsOk(Code))
            {
                Fail(FString::Printf(TEXT("Il server ha restituito HTTP %d."), Code));
                return;
            }

            const FString Content = Response->GetContentAsString();
            TSharedPtr<FJsonObject> Payload;
            FString Status;
            FString Reply;
            if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(Content), Payload)
                || !Payload.IsValid()
                || !Payload->TryGetStringField(TEXT("status"), Status)
                || !Payload->TryGetStringField(TEXT("reply"), Reply))
            {
                Fail(TEXT("Risposta JSON del server non valida."));
                return;
            }

            Completed.Broadcast(Status, Reply, FString());
            SetReadyToDestroy();
        }
    );

    if (!Request->ProcessRequest() && !bReceivedCompletion)
    {
        Request->OnProcessRequestComplete().Unbind();
        Fail(TEXT("Impossibile avviare la richiesta HTTP."));
    }
}

void ULocalTextQuery::Fail(const FString& Error)
{
    Failed.Broadcast(FString(), FString(), Error);
    SetReadyToDestroy();
}
