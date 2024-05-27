from fastapi import FastAPI

from database import init_db
from measurements import router

app = FastAPI()
app.include_router(router.router)

init_db()

@app.get("/")
def read_root():
    return {"Hello": "World"}

