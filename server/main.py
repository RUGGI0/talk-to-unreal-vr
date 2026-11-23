from typing import Annotated, Literal

from fastapi import FastAPI
from pydantic import BaseModel, StringConstraints


app = FastAPI(title="VR Project API")


class Query(BaseModel):
    text: Annotated[
        str, StringConstraints(strict=True, strip_whitespace=True, min_length=1)
    ]


class EchoResponse(BaseModel):
    status: Literal["echo"] = "echo"
    reply: str


@app.get("/health")
def health() -> dict[str, str]:
    return {"status": "ok"}


@app.post("/query")
def query(request: Query) -> EchoResponse:
    return EchoResponse(reply=f"Ricevuto: {request.text}")
