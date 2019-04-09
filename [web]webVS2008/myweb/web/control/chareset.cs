namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class chareset : UserControl
    {
        protected Button btnedit;
        protected DropDownList ddchalist;
        protected RadioButton rbgold;
        protected RadioButton rbmoney;
        protected RequiredFieldValidator Requiredfieldvalidator6;

        private void btnedit_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int chaidx = int.Parse(this.ddchalist.SelectedValue.ToString());
            int count = int.Parse(base.Application["game.charesetcount"].ToString());
            int point = int.Parse(base.Application["game.charesetgivepoint"].ToString());
            int flv = int.Parse(base.Application["game.charesetflv"].ToString());
            int lvstep = int.Parse(base.Application["game.charesetlvstep"].ToString());
            int fmoney = int.Parse(base.Application["game.charesetfmoney"].ToString());
            int moneystep = int.Parse(base.Application["game.charesetmoneystep"].ToString());
            int paymode = this.rbgold.Checked ? 1 : 0;
            string str = new WebLogic().chareset(base.Session["userid"].ToString(), useridx, chaidx, count, point, flv, lvstep, fmoney, moneystep, paymode);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name+'['+convert(varchar,webchareset)+'锣]' as character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
            }
        }
    }
}
