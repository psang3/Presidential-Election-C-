// Author: Pathik Sangani
// Date: November-11-2024
/* Description : A C++ program that analyzes U.S. election data from
  2012, 2016, and 2020, allowing users to view national, state, and
 candidate-specific voting results. */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

// Constants for state names
const string STATES[] = {
    "ALABAMA", "ALASKA", "ARIZONA", "ARKANSAS", "CALIFORNIA",
    "COLORADO", "CONNECTICUT", "DELAWARE", "FLORIDA", "GEORGIA",
    "HAWAII", "IDAHO", "ILLINOIS", "INDIANA", "IOWA",
    "KANSAS", "KENTUCKY", "LOUISIANA", "MAINE", "MARYLAND",
    "MASSACHUSETTS", "MICHIGAN", "MINNESOTA", "MISSISSIPPI", "MISSOURI",
    "MONTANA", "NEBRASKA", "NEVADA", "NEW HAMPSHIRE", "NEW JERSEY",
    "NEW MEXICO", "NEW YORK", "NORTH CAROLINA", "NORTH DAKOTA", "OHIO",
    "OKLAHOMA", "OREGON", "PENNSYLVANIA", "RHODE ISLAND", "SOUTH CAROLINA",
    "SOUTH DAKOTA", "TENNESSEE", "TEXAS", "UTAH", "VERMONT",
    "VIRGINIA", "WASHINGTON", "WASHINGTON DC", "WEST VIRGINIA", "WISCONSIN",
    "WYOMING"
};

const int NUM_STATES = 51;

// Class to store a single set of votes
class Votes{
private:
    string state;
    string county;
    string candidate;
    string party;
    int voteCount;

public:
    // Constructors
    Votes() : voteCount(0){}
    Votes(string s, string c, string can, string p, int v) :
        state(s), county(c), candidate(can), party(p), voteCount(v){}

    // Getters
    string getState() const { return state; }
    string getCounty() const { return county; }
    string getCandidate() const { return candidate; }
    string getParty() const { return party; }
    int getVoteCount() const { return voteCount; }
};

// Class to store candidate summary information
class CandidateSummary {
    public:
        string name;
        string party;
        int totalVotes;

        CandidateSummary(string n, string p) : name(n), party(p), totalVotes(0){}

        bool operator<(const CandidateSummary& other) const {
            return totalVotes > other.totalVotes;
        }
};

// Function prototypes
vector<Votes> readVotesFromFile(const string& filename);
void showDataOverview(const vector<Votes>& votes);
void showNationalResults(const vector<Votes>& votes);
void showStateResults(const vector<Votes>& votes);
void showCandidateResults(const vector<Votes>& votes);
void showCountySearch(const vector<Votes>& votes);
string toUpper(string str);
vector<CandidateSummary> getCandidateSummaries(const vector<Votes>& votes);

// Main Function
int main(){
    string filename;
    cout << "Enter file to use: ";
    getline(cin, filename);

    vector<Votes> votes = readVotesFromFile(filename);

    while(true){
        cout << "\nSelect a menu option:\n";
        cout << "  1. Data overview\n";
        cout << "  2. National results\n";
        cout << "  3. State results\n";
        cout << "  4. Candidate results\n";
        cout << "  5. County search\n";
        cout << "  6. Exit\n";
        cout << "Your choice: ";

        int choice;
        cin >> choice;
        cin.ignore(); // clear newline from input buffer

        switch(choice){
            case 1:
                showDataOverview(votes);
                break;
            case 2:
                showNationalResults(votes);
                break;
            case 3:
                showStateResults(votes);
                break;
            case 4:
                showCandidateResults(votes);
                break;
            case 5:
                showCountySearch(votes);
                break;
            case 6:
                return 0;
            default:
                break;
        } 
    }
}

// reads and parses election data from csv file into vector of vote objects
vector<Votes> readVotesFromFile(const string& filename){
    vector<Votes> votes;
    ifstream file(filename);
    string state, county, candidate, party, votesStr;

    while(!file.eof()){
        getline(file, state, ',');
        if (file.eof()) break;

        getline(file, county, ',');
        getline(file, candidate, ',');
        getline(file, party, ',');
        getline(file, votesStr);

        int voteCount = stoi(votesStr);
        votes.emplace_back(state, county, candidate, party, voteCount);
    }
    return votes;
}

// converts string to uppercase for case-insensitive comparison
string toUpper(string str){
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// creates summary of total votes for each candidate
vector<CandidateSummary> getCandidateSummaries(const vector<Votes>& votes){
    vector<CandidateSummary> summaries;

    for (const Votes& vote : votes){
        bool found = false;
        for (CandidateSummary& summary : summaries){
            if(summary.name == vote.getCandidate()){
                summary.totalVotes += vote.getVoteCount();
                found = true;
                break;
            }
        }
        if(!found){
            summaries.emplace_back(vote.getCandidate(), vote.getParty());
            summaries.back().totalVotes = vote.getVoteCount();
        }
    }
    sort(summaries.begin(), summaries.end());
    return summaries;
}

// displays total number of records and votes in the dataset
void showDataOverview(const vector<Votes>& votes) {
    int totalVotes = 0;
    for (const Votes& vote : votes) {
        totalVotes += vote.getVoteCount();
    }
    
    cout << "Number of election records: " << votes.size() << endl;
    cout << "Total number of votes recorded: " << totalVotes << endl;
}

// show national vote totals for each candidate, sorted by numer of votes
void showNationalResults(const vector<Votes>& votes){
    vector<CandidateSummary> summaries = getCandidateSummaries(votes);

    for(const CandidateSummary& summary : summaries){
        cout << left << setw(20) << summary.name
             << left << setw(15) << summary.party
             << right << setw(10) << summary.totalVotes << endl;
    }
}

// Displays graphical bar chart of votes in user-specified state
void showStateResults(const vector<Votes>& votes){
    string stateInput;
    cout << "Enter state: ";
    getline(cin , stateInput);
    string state = toUpper(stateInput);

    vector<CandidateSummary> stateSummaries;
    for (const Votes& vote : votes){
        if (vote.getState() == state){
            bool found = false;
            for (CandidateSummary& summary : stateSummaries){
                if (summary.name == vote.getCandidate()){
                    summary.totalVotes += vote.getVoteCount();
                    found = true;
                    break;
                }
            }
            if(!found){
                stateSummaries.emplace_back(vote.getCandidate(), vote.getParty());
                stateSummaries.back().totalVotes = vote.getVoteCount();
            }
        }
    }
    sort(stateSummaries.begin(), stateSummaries.end());

    for(const CandidateSummary& summary : stateSummaries){
        cout << left << setw(20) << summary.name;
        int bars = round(summary.totalVotes / 150000.0);
        cout << string(bars, '|') << endl;
    }
}

// Shows state-by-state results for specified candidate
void showCandidateResults(const vector<Votes>& votes) {
    string candidateSearch;
    cout << "Enter candidate: ";
    getline(cin, candidateSearch);
    candidateSearch = toUpper(candidateSearch);
    
    string candidateName;
    for (const Votes& vote : votes) {
        if (toUpper(vote.getCandidate()).find(candidateSearch) != string::npos) {
            candidateName = vote.getCandidate();
            break;
        }
    }
    
    vector<pair<string, pair<int, int>>> stateResults(NUM_STATES);
    for (int i = 0; i < NUM_STATES; i++) {
        stateResults[i].first = STATES[i];
        stateResults[i].second.first = 0;  // Candidate votes
        stateResults[i].second.second = 0; // Total votes
    }
    
    for (const Votes& vote : votes) {
        for (int i = 0; i < NUM_STATES; i++) {
            if (vote.getState() == STATES[i]) {
                if (vote.getCandidate() == candidateName) {
                    stateResults[i].second.first += vote.getVoteCount();
                }
                stateResults[i].second.second += vote.getVoteCount();
                break;
            }
        }
    }
    
    double bestPercentage = 0.0;
    string bestState;
    
    for (const auto& result : stateResults) {
        cout << left << setw(20) << result.first;
        cout << right << setw(10) << result.second.first;
        cout << right << setw(10) << result.second.second;
        
        double percentage = 0.0;
        if (result.second.second > 0) {
            percentage = (100.0 * result.second.first) / result.second.second;
            if (percentage > bestPercentage) {
                bestPercentage = percentage;
                bestState = result.first;
            }
        }
        
        cout << right << setw(7) << fixed << setprecision(1) << percentage << "%" << endl;
    }
    
    cout << "The best state for " << candidateName << " is " << bestState << endl;
}

//Displays all voting results for countries matching search term
void showCountySearch(const vector<Votes>& votes){
    string countySearch;
    cout << "Enter county: ";
    getline(cin, countySearch);
    countySearch = toUpper(countySearch);

    for(const Votes& vote : votes){
        if(toUpper(vote.getCounty()).find(countySearch) != string::npos){
            cout << left << setw(40) << (vote.getCounty() + ", " + vote.getState())
                 << left << setw(20) << vote.getCandidate()
                 << right << setw(10) << vote.getVoteCount() << endl;
        }
    }
}