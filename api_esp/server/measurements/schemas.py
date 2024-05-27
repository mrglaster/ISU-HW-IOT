from datetime import datetime
from typing import Union, List

from pydantic import BaseModel


class TemperatureValue(BaseModel):
    value: float
    location: str
    device_id: str
    timestamp: Union[datetime, None] = None


class DataAnswerModel(BaseModel):
    values: List[float]
    location: str
