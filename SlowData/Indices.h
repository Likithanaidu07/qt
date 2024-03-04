#ifndef INDICES_H
#define INDICES_H

#include <string>
#include <ctime>

class Indices {
public:
    static Indices Nifty50;
    static Indices BankNifty;
    static Indices IndiaVIX;

    std::time_t LastUpdateTime;
    std::string IndexName;
    int IndexValue;
    int HighIndexValue;
    int LowIndexValue;
    int OpeningIndex;
    int ClosingIndex;
    int PercentChange;
    int YearlyHigh;
    int YearlyLow;
    int NoOfUpmoves;
    int NoOfDownmoves;
    double MarketCapitalisation;
    char NetChangeIndicator;
    char FILLER;

    // static int Deserialize(const char* buffer, int index) {
    //     int recordCount = 0;

    //     recordCount = static_cast<int>((static_cast<int>(buffer[index++]) << 8) | buffer[index++]);

    //     for (int k = 0; k < recordCount; k++) {
    //         std::string name = "";
    //         for (int i = 0; i < 21; i++) {
    //             name += static_cast<char>(buffer[index++]);
    //         }

    //         index++;

    //         Indices* idValue = nullptr;
    //         if (name == "Nifty 50") {
    //             if (Nifty50 == nullptr) {
    //                 Nifty50 = new Indices();
    //             }
    //             idValue = &Nifty50;
    //         } else if (name == "Nifty Bank") {
    //             if (BankNifty == nullptr) {
    //                 BankNifty = new Indices();
    //             }
    //             idValue = &BankNifty;
    //         } else if (name == "India VIX") {
    //             if (IndiaVIX == nullptr) {
    //                 IndiaVIX = new Indices();
    //             }
    //             idValue = &IndiaVIX;
    //         }

    //         if (idValue == nullptr) {
    //             index += 50;
    //             continue;
    //         }

    //         idValue->LastUpdateTime = std::time(nullptr);
    //         idValue->IndexName = name;

    //         idValue->IndexValue = (static_cast<int>(buffer[index++]) << 24) |
    //                               (static_cast<int>(buffer[index++]) << 16) |
    //                               (static_cast<int>(buffer[index++]) << 8) |
    //                               buffer[index++];

    //         idValue->HighIndexValue = (static_cast<int>(buffer[index++]) << 24) |
    //                                   (static_cast<int>(buffer[index++]) << 16) |
    //                                   (static_cast<int>(buffer[index++]) << 8) |
    //                                   buffer[index++];

    //         idValue->LowIndexValue = (static_cast<int>(buffer[index++]) << 24) |
    //                                  (static_cast<int>(buffer[index++]) << 16) |
    //                                  (static_cast<int>(buffer[index++]) << 8) |
    //                                  buffer[index++];

    //         idValue->OpeningIndex = (static_cast<int>(buffer[index++]) << 24) |
    //                                 (static_cast<int>(buffer[index++]) << 16) |
    //                                 (static_cast<int>(buffer[index++]) << 8) |
    //                                 buffer[index++];

    //         idValue->ClosingIndex = (static_cast<int>(buffer[index++]) << 24) |
    //                                 (static_cast<int>(buffer[index++]) << 16) |
    //                                 (static_cast<int>(buffer[index++]) << 8) |
    //                                 buffer[index++];

    //         idValue->PercentChange = (static_cast<int>(buffer[index++]) << 24) |
    //                                  (static_cast<int>(buffer[index++]) << 16) |
    //                                  (static_cast<int>(buffer[index++]) << 8) |
    //                                  buffer[index++];

    //         idValue->YearlyHigh = (static_cast<int>(buffer[index++]) << 24) |
    //                               (static_cast<int>(buffer[index++]) << 16) |
    //                               (static_cast<int>(buffer[index++]) << 8) |
    //                               buffer[index++];

    //         idValue->YearlyLow = (static_cast<int>(buffer[index++]) << 24) |
    //                              (static_cast<int>(buffer[index++]) << 16) |
    //                              (static_cast<int>(buffer[index++]) << 8) |
    //                              buffer[index++];

    //         idValue->NoOfUpmoves = (static_cast<int>(buffer[index++]) << 24) |
    //                                (static_cast<int>(buffer[index++]) << 16) |
    //                                (static_cast<int>(buffer[index++]) << 8) |
    //                                buffer[index++];

    //         idValue->NoOfDownmoves = (static_cast<int>(buffer[index++]) << 24) |
    //                                  (static_cast<int>(buffer[index++]) << 16) |
    //                                  (static_cast<int>(buffer[index++]) << 8) |
    //                                  buffer[index++];

    //         idValue->MarketCapitalisation = 0;
    //         index += 8;

    //         idValue->NetChangeIndicator = static_cast<char>(buffer[index++]);
    //         idValue->FILLER = static_cast<char>(buffer[index++]);
    //     }

    //     return index;
    // }
};

Indices Indices::Nifty50;
Indices Indices::BankNifty;
Indices Indices::IndiaVIX;


#endif // INDICES_H
