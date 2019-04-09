namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using web;

    public class smalltop : UserControl
    {
        protected string strhero;

        private void InitializeComponent()
        {
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new system().loadConfig(0);
            if (!this.Page.IsPostBack)
            {
                DataProviders providers = new DataProviders();
                SqlDataReader reader = providers.ExecuteSqlDataReader("select top 10 character_name,character_grade,webchareset from mhgame..tb_character  where substring(character_name,1,1)!='@' order by webchareset desc,character_grade desc,character_EXPOINT desc");
                for (int i = 1; reader.Read(); i++)
                {
                    object strhero = this.strhero;
                    this.strhero = string.Concat(new object[] { strhero, "<tr><td width=15% align=left><img src=images/top_", i, ".gif></td><td width=50%><strong>", new system().ConvertToBig5(reader["character_name"].ToString(), 950), "</strong></td><td width=30% align=center>", reader["character_grade"].ToString(), "級", reader["webchareset"], "轉</td></tr>" });
                }
                reader.Close();
                providers.CloseConn();
            }
        }
    }
}
