#pragma once
#include "LSTM_cell.h"
class StockPredictor :
    public LSTM_cell
{
    //reads a csv file with inputs on format priceDay1,priceDay2, priceDay3...
    //we need a format validator?
    //ar mai fi posibil sa citim si de pe internet direct.
    StockPredictor(FILE csvInput)
    {
    }
};

