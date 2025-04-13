using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace CNT
{
    public class ConfigObject : IEquatable<ConfigObject>
    {
        public string Name { get; set; }
        public string Value { get; set; }

        public bool Equals(ConfigObject other)
        {
            if (other is null) return false;
            return Name == other.Name && Value == other.Value;
        }

        public override bool Equals(object obj) => Equals(obj as ConfigObject);
        public override int GetHashCode() => HashCode.Combine(Name, Value);
    }

    public class ConfigManager : IEnumerable<ConfigObject>
    {
        private readonly List<ConfigObject> _configs = new();
        private string _currentPath = "";

        // 构造函数
        public ConfigManager(string path = "")
        {
            if (!string.IsNullOrEmpty(path))
            {
                LoadFile(path);
            }
        }

        // 索引器
        public string this[string name]
        {
            get => GetValue(name) ?? "";
            set
            {
                var item = _configs.FirstOrDefault(c => c.Name == name);
                if (item != null) item.Value = value;
                else _configs.Add(new ConfigObject { Name = name, Value = value });
            }
        }

        // 文件扩展名验证
        private bool ValidateExtension(string filename, string expected)
        {
            var ext = Path.GetExtension(filename);
            return ext.Equals(expected, StringComparison.OrdinalIgnoreCase);
        }

        // 字符串处理
        private string Trim(string str) => str.Trim();

        // 解析配置行
        private ConfigObject ParseLine(string line)
        {
            line = Trim(line);
            if (string.IsNullOrEmpty(line) || line.StartsWith("#"))
                return new ConfigObject();

            var parts = line.Split(new[] { '=' }, 2);
            return parts.Length == 2
                ? new ConfigObject { Name = Trim(parts[0]), Value = Trim(parts[1]) }
                : new ConfigObject();
        }

        // 加密解密（异或算法）
        private static string XorCipher(string data, byte key = 0xBB)
        {
            var bytes = Encoding.UTF8.GetBytes(data);
            return new string(bytes.Select(b => (char)(b ^ key)).ToArray());
        }

        // 二进制序列化
        private byte[] SerializeBinary()
        {
            using var ms = new MemoryStream();
            using var writer = new BinaryWriter(ms);

            foreach (var obj in _configs)
            {
                writer.Write(obj.Name);
                writer.Write(obj.Value);
            }
            return ms.ToArray();
        }

        // 二进制反序列化
        private void DeserializeBinary(byte[] data)
        {
            using var ms = new MemoryStream(data);
            using var reader = new BinaryReader(ms);

            _configs.Clear();
            while (ms.Position < ms.Length)
            {
                _configs.Add(new ConfigObject
                {
                    Name = reader.ReadString(),
                    Value = reader.ReadString()
                });
            }
        }

        // 文件操作
        public bool LoadText(string filename)
        {
            if (!ValidateExtension(filename, ".cntconfig"))
                throw new InvalidOperationException("Invalid text config extension");

            _configs.Clear();
            foreach (var line in File.ReadLines(filename))
            {
                var obj = ParseLine(line);
                if (!string.IsNullOrEmpty(obj.Name))
                    _configs.Add(obj);
            }
            return true;
        }

        public bool SaveText(string filename)
        {
            if (!ValidateExtension(filename, ".cntconfig"))
                throw new InvalidOperationException("Invalid text config extension");

            File.WriteAllLines(filename,
                _configs.Select(c => $"{c.Name}={c.Value}"));
            return true;
        }

        public bool LoadBinary(string filename)
        {
            if (!ValidateExtension(filename, ".cntconfigbin"))
                throw new InvalidOperationException("Invalid binary extension");

            var encrypted = File.ReadAllText(filename);
            var decrypted = XorCipher(encrypted);
            DeserializeBinary(Encoding.UTF8.GetBytes(decrypted));
            return true;
        }

        public bool SaveBinary(string filename)
        {
            if (!ValidateExtension(filename, ".cntconfigbin"))
                throw new InvalidOperationException("Invalid binary extension");

            var data = XorCipher(Encoding.UTF8.GetString(SerializeBinary()));
            File.WriteAllText(filename, data);
            return true;
        }

        public bool LoadFile(string filename)
        {
            if (ValidateExtension(filename, ".cntconfig")) return LoadText(filename);
            if (ValidateExtension(filename, ".cntconfigbin")) return LoadBinary(filename);
            throw new InvalidOperationException("Unsupported file format");
        }

        // 配置项操作
        public void Add(ConfigObject obj) => _configs.Add(obj);
        public void Add(string name, string value) => _configs.Add(new ConfigObject { Name = name, Value = value });

        public bool RemoveByName(string name)
        {
            var count = _configs.RemoveAll(c => c.Name == name);
            return count > 0;
        }

        public bool RemoveByValue(string value)
        {
            var count = _configs.RemoveAll(c => c.Value == value);
            return count > 0;
        }

        public bool RemoveByIndex(int index)
        {
            if (index < 0 || index >= _configs.Count) return false;
            _configs.RemoveAt(index);
            return true;
        }

        public void Clear() => _configs.Clear();

        // 查询方法
        public string GetValue(string name) =>
            _configs.FirstOrDefault(c => c.Name == name)?.Value ?? "";

        public string GetName(string value) =>
            _configs.FirstOrDefault(c => c.Value == value)?.Name ?? "";

        public bool Contains(string name) =>
            _configs.Any(c => c.Name == name);

        public int Count => _configs.Count;

        // 迭代器实现
        public IEnumerator<ConfigObject> GetEnumerator() => _configs.GetEnumerator();
        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() => GetEnumerator();
    }
}