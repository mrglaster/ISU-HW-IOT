from sqlalchemy import Column, Integer, String, DateTime, Float

from database import Base


class TemperatureValueModel(Base):
    __tablename__ = "temperature_values"

    id = Column(Integer, primary_key=True, index=True)
    device_id = Column(String)
    value = Column(Float)
    location = Column(String)
    timestamp = Column(DateTime)
