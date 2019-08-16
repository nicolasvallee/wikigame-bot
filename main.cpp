#include <iostream>
#include <fstream>
#include <regex>
#include <cstdlib>
#include <queue>

struct Page
{
    std::string name;
    int id;
    int previousId;
};

std::vector<Page> pagesSeen;


std::vector<std::string> getAdjacentPages(std::string page)
{
    std::string command = "curl https://en.wikipedia.org/wiki/" + page + " -o ./html_files/" + page + ".html";
    std::system(command.c_str());
    std::ifstream file;
    file.open("./html_files/" + page + ".html");
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string html =  buffer.str();

    std::regex expr("<a href=\"\\/wiki\\/([^\":]+?)\" title");
    std::sregex_iterator it(html.begin(), html.end(), expr);
    std::sregex_iterator fin;

    std::vector<std::string> adj;
    while(it != fin)
    {
        //std::cout << it-> str(1) << '\n';
        adj.push_back(it->str(1));
        it++;
    }

    file.close();
    return adj;
}

void printPath(Page p)
{
    std::cout << p.name << '\n';
    while(p.previousId != -1)
    {
        p = pagesSeen[p.previousId];
        std::cout << p.name << '\n';
    }
}

void BFS(std::string startPage, std::string endPage)
{
    std::queue<Page> Q;
    Q.push({startPage, 0, -1});
    pagesSeen.push_back({startPage, 0, -1});
    while(!Q.empty())
    {
        Page cur = Q.front();
        Q.pop();
        if(cur.name == endPage)
        {
            printPath(cur);
            break;
        }
        std::cout << "cur = " << cur.name << '\n';

        std::vector<std::string> adjacentPages = getAdjacentPages(cur.name);
        for(std::string next : adjacentPages)
        {
            bool seen = false;
            for(int i = 0; i < pagesSeen.size(); i++)
            {
                if(pagesSeen[i].name == next)
                {
                    seen = true;
                    break;
                }
            }
            if(!seen)
            {
                pagesSeen.push_back({next, pagesSeen.size(), cur.id});
                Q.push({next, pagesSeen.size() - 1, cur.id});
            }
        }
    }
}

int main()
{
    std::string startPage, endPage;
    std::cout << "Enter start page : " << '\n';
    std::cin >> startPage;
    std::cout << "Enter end page : " << '\n';
    std::cin >> endPage;

    BFS(startPage, endPage);

    return 0;
}
