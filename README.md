# 项目说明
## 1. 项目介绍

## 2. 项目架构
### 2.1 模块介绍
### 2.2 模块架构
#### 2.2.1 命令管理器
Command → Transaction → Document → Event → View
```mermaid
graph TD
    A["CommandManager"] -->|execute(command)| B["Transaction\n(开启记录)"]
    B --> C["command->execute()"]
    C --> D["context().selectionManager().PickXxx()"]
    C --> E["document->addObject(shape)"]
    E --> F["XCAF写入"]
    E --> G["Transaction.recordAdd(id, shape)"]
    G --> H["DocumentEvent(ObjectAdded)"]
    H --> I["View::onObjectAdded(id, shape)"]
    I --> J["labelById() → XCAFPrs_AISObject"]
    J --> K["AISContext->Display()"]
    B --> L["commitTransaction()"]
    L --> M["undoStack.push(txn)"]
```   