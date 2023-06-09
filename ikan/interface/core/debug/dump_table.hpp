//
//  dump_table.h
//  ikan
//
//  Created by Ashish . on 06/04/23.
//

#pragma once

namespace ikan {
  
  struct Table {
    struct TableColumn {
      uint32_t max_entry_width = 0;
      uint32_t total_entries = 0;
      std::vector<std::string> col_entries;
      
      void AddEntry(const std::string& entry) {
        col_entries.push_back(entry);
        max_entry_width = std::max(max_entry_width, (uint32_t)entry.size());
        total_entries++;
      }
    };
    
    Table(const std::string& title, uint32_t num_col, bool header = true);

    void AddRow(const std::vector<std::string>& rows);
    void AddColumn(const std::vector<std::string>& cols);
    void Dump(Logger::Level level, const std::string& module_name = "");
    
    std::vector<std::string> Rows();
    
    std::vector<TableColumn> table_entries;
    uint32_t num_cols = 0;
    uint32_t num_rows = 0;
    uint32_t num_cols_added = 0;
    
    std::string title = "Table Data";
    bool header = true;
  };
  
} // namespace ikan
